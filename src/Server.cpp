#include "Server.hpp"
#include "Logger.hpp"
#include <errno.h>
#include <system_error>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <regex>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <mutex>
#include <signal.h>

namespace ServerSide {

const int successes = 0;

Server::Server(){}

void MyParallelServer::open(int port, ClientHandle::ClientHandlerGenerator & c){
    int server_socket;

    try {

        int opt = 1;
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        Logger::log(Logger::Level::Info, "opening the server");

        if (server_socket < 0) {
            Logger::log(Logger::Level::Error, "cant open the socket");
            throw std::system_error{errno, std::system_category()};
        }

        sockaddr_in address;
        address.sin_family = AF_INET; 
        address.sin_addr.s_addr = INADDR_ANY; 
        address.sin_port = htons( port );

        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        { 
            Logger::log(Logger::Level::Error, "set socket option failed");
            throw std::system_error{errno, std::system_category()};
        }

        Logger::log(Logger::Level::Info, "binding server port " + std::to_string(port));
        if (bind(server_socket, (struct sockaddr *)&address, sizeof(address))<0) 
        { 
            Logger::log(Logger::Level::Error, "bind failed");
            throw std::system_error{errno, std::system_category()};
        }

        if (listen(server_socket, backlogSize) < 0) 
        { 
            Logger::log(Logger::Level::Error, "listen failed");
            throw std::system_error{errno, std::system_category()};
        }

        sockaddr new_client_address;
        socklen_t addrlen= sizeof(new_client_address);
        
        int new_client_socket = 0;
 
        fd_set fds;
		FD_ZERO (&fds);
		FD_SET (server_socket, &fds);
        // struct timeval timeout;      
        // timeout.tv_sec = 0;
        // timeout.tv_usec = 100;

        m_clientHandlers.resize(threadPoolSize);
        m_clientSockets.resize(threadPoolSize);
        m_avialbleTasks.resize(threadPoolSize);
        for (size_t i = 0; i < threadPoolSize; ++i) {
            m_avialbleTasks [i] = threadPoolSize - i - 1;
            m_clientHandlers[i] = c.generate();
            m_threads.push_back(std::thread(&MyParallelServer::clientThreadFunc, this));
        }

        bool conectionRefused = false;
        while (m_running) {
                        
            int res = select(server_socket + 1, &fds, NULL, NULL, NULL);
            if (res < 0) {
               if (errno!=EINTR){
                Logger::log(Logger::Level::Error, "select error");
                throw std::system_error{errno, std::system_category()};
               } else {
                   // signal
                   Logger::log(Logger::Level::Debug, "res is -1 errno=EINTR");
                   continue;
               }
            } else if (res == 0) {
                // timout
                Logger::log(Logger::Level::Debug, "res is 0");
                continue;
            }

            size_t index;
            {
                std::lock_guard<std::mutex> guard(m_avialbleTasksMutex);
                conectionRefused = (m_avialbleTasks.empty());
                index = m_avialbleTasks.back();
                m_avialbleTasks.pop_back();
            }

            new_client_socket = accept(server_socket, (struct  sockaddr  *)&new_client_address, &addrlen);
            
            if (new_client_socket < 0) { 
                Logger::log(Logger::Level::Error, "accept error");
                throw std::system_error{errno, std::system_category()};
            }

            if (conectionRefused) {
                close(new_client_socket);
                Logger::log(Logger::Level::Info, " connection refused");
                continue;
            }

            Logger::log(Logger::Level::Info, "using index: " + std::to_string(index) + " client socket" + std::to_string(new_client_socket));

            m_clientSockets[index] = new_client_socket;
        
            {
                std::lock_guard<std::mutex> guard(m_taskQueueMutex);
                m_taskQueue.push_back(index);
                m_cv.notify_one();
                // signal thread to act
            }


        }
        close(server_socket);
        Logger::log(Logger::Level::Info, "stopping server");
        m_cv.notify_all();
        for (size_t  i = 0; i < m_threads.size(); ++i) {
            if (m_threads[i].joinable()){
                m_threads[i].join();
            }
        }
        Logger::log(Logger::Level::Info, "server stopped");


    } catch (std::system_error& e){
        e.what();
    }


}


void MyParallelServer::acceptClient(ClientHandle::ClientHandler & c, int & socket) {
    std::string debugstr = "acceptClient(" + std::to_string(socket) + "): ";
    Logger::log(Logger::Level::Info, debugstr + "accepted connection");

    char buf[1024];
    int bytesRead;


    bool running = true;
    bool didGetAnyInput = false;

    while (running) {
        std::stringstream ins;
        auto start = std::chrono::system_clock::now();
        Logger::log(Logger::Level::Debug, debugstr + "start reading message");

        while (true) {
            bytesRead = recv(socket, buf, sizeof(buf) - 1, MSG_DONTWAIT);
            int readerrno = errno;
            bool wouldblock = ((bytesRead < 0) && (readerrno == EWOULDBLOCK));

            if (bytesRead > 0){
                buf[bytesRead] = 0;
                std::string msg = buf;
                Logger::log(Logger::Level::Debug, debugstr + "recived:" + msg);
                didGetAnyInput = true;
            }

            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end-start;
            size_t seconds = diff.count() ;

            if ((seconds > timeoutNoReading) && (didGetAnyInput == false)) {
                Logger::log(Logger::Level::Info, debugstr + "5 seconds without any input - closing the connection");
                running = false;
                break;
            }
            if (seconds > noValidMessageTimeout) {
                Logger::log(Logger::Level::Info, debugstr + "timeout reading - didnt find valid message");
                running = false;
                break;
            }

            if (wouldblock){
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                continue;
            }
            if (bytesRead == 0) {
                Logger::log(Logger::Level::Info, debugstr + "closed by peer");
                running = false;
                break;
            }

            if (bytesRead < 0) {  
                Logger::log(Logger::Level::Error, debugstr + "exception while reading");
                running = false;
                break;
            }

            if ((size_t)ins.tellp() > maxMsgSize - sizeof(buf)) {
                Logger::log(Logger::Level::Error, debugstr + "message too big" + std::to_string((size_t)ins.tellp()));
                running = false;
                break;
            }
            ins.seekg(std::ios_base::beg);
            ins.write(buf,bytesRead);

            if (ins.str().find(c.getSyncString()) != std::string::npos) {
                break;
            }     
        }               
                
        if (!running) {
            break;
        }

        std::stringstream outs;
    
        ClientHandle::GraphSolverStatus status = c.handleClient(ins, outs);
        if (status != successes) {
            running = false;
        }

        int byteWriten = 0;
        start = std::chrono::system_clock::now();

        while (outs) {
            byteWriten = write(socket, outs.str().c_str(), outs.str().length());
            if (byteWriten < 0) {
                Logger::log(Logger::Level::Error, debugstr + "exception while writing");
                break;
            }
            if (byteWriten == 0) {
                break;
            }
            Logger::log(Logger::Level::Debug, debugstr + "wrting: " + outs.str());


            outs.seekp(byteWriten, std::ios_base::cur);
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end-start;

            size_t seconds = diff.count();
            if (seconds > noValidMessageTimeout) {
                Logger::log(Logger::Level::Info, debugstr + "timeout writing");
                running = false;
                break;
            }
        }
        if (status != ClientHandle::GraphSolverStatus::successes) {
            running = false;
        }
        c.update(running);
        
    }

}



void  MyParallelServer::stop() {
    m_running = false;
}

MyParallelServer::MyParallelServer() : m_running(true){

}
void MyParallelServer::clientThreadFunc() {
    size_t task;
    while (m_running) {
        {
            std::unique_lock<std::mutex> lock(m_taskQueueMutex);     
            m_cv.wait(lock);
            if (!m_running) {
                break;
            }
            if (m_taskQueue.empty()) {
                continue;
            }
            task = m_taskQueue.back();
            m_taskQueue.pop_back(); 
        }
        acceptClient(*(m_clientHandlers[task].get()), m_clientSockets[task]);
        {
            std::lock_guard<std::mutex> guard(m_avialbleTasksMutex);
            m_avialbleTasks.push_back(task);
            Logger::log(Logger::Level::Debug, "closing socket in index:   " + std::to_string(task) + "socket number" + std::to_string(m_clientSockets[task]));
            close(m_clientSockets[task]);
        }
    }

}





}