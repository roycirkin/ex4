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
    int sockfd;

    try {

        int opt = 1;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        Logger::log(Logger::Level::Info, "opening the server");

        if (sockfd < 0) {
            Logger::log(Logger::Level::Error, "cant open the socket");
            throw std::system_error{errno, std::system_category()};
        }

        sockaddr_in address;
        address.sin_family = AF_INET; 
        address.sin_addr.s_addr = INADDR_ANY; 
        address.sin_port = htons( port );

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        { 
            Logger::log(Logger::Level::Error, "set socket option failed");
            throw std::system_error{errno, std::system_category()};
        }

        Logger::log(Logger::Level::Info, "binding server port " + std::to_string(port));
        if (bind(sockfd, (struct sockaddr *)&address, sizeof(address))<0) 
        { 
            Logger::log(Logger::Level::Error, "bind failed");
            throw std::system_error{errno, std::system_category()};
        }

        if (listen(sockfd, backlogSize) < 0) 
        { 
            Logger::log(Logger::Level::Error, "listen failed");
            throw std::system_error{errno, std::system_category()};
        }

        socklen_t addrlen;
        struct sockaddr pear_address;
        int newSocket = 0;
 
		/* BANG! we can get SIGTERM at this point. */
        fd_set fds;
		FD_ZERO (&fds);
		FD_SET (sockfd, &fds);

        m_clientHandlers.resize(threadPoolSize);
        m_clientSockets.resize(threadPoolSize);
        m_avialbleTasks.resize(threadPoolSize);
        for (size_t i = 0; i < threadPoolSize; ++i) {
            m_avialbleTasks [i] = threadPoolSize - i - 1;
            m_clientHandlers[i] = c.generate();
            m_threads.push_back(std::thread(&MyParallelServer::clientThreadFunc, this));
        }

        // struct timespec timeout;
        // timeout.tv_sec = 0;
        // timeout.tv_nsec = 100000;

        sigset_t orig_mask;
        sigset_t mask;

        sigemptyset (&mask);
        sigaddset (&mask, SIGTERM);
    
        if (sigprocmask(SIG_BLOCK, &mask, &orig_mask) < 0) {
            //error
        }

        while (m_running) {

            int res = select (sockfd + 1, &fds, NULL, NULL, NULL);
            if (res < 0 && errno != EINTR) {
                Logger::log(Logger::Level::Error, "select failed");
                break;
            }
            else if (res == 0) {
                continue;
            }


            Logger::log(Logger::Level::Error, "after the continue");

            size_t index;
            {
                std::lock_guard<std::mutex> guard(m_avialbleTasksMutex);
                index = m_avialbleTasks.back();
                m_avialbleTasks.pop_back();
            }
            
            Logger::log(Logger::Level::Info, "using index " + std::to_string(index));

            newSocket = accept(sockfd, &pear_address, &addrlen);
            
            if (newSocket < 0) { 
                Logger::log(Logger::Level::Error, "connection refused");
                throw std::system_error{errno, std::system_category()};
            }

            m_clientSockets[index] = newSocket;
        
            {
                std::lock_guard<std::mutex> guard(m_taskQueueMutex);
                m_taskQueue.push_back(index);
                m_cv.notify_one();
                // signal thread to act
            }

            //ClientHandle::ClientHandler* clientHandler = (m_clientHandlers[index]).get();
            //acceptClient(*clientHandler, m_clientSockets[index]);
            //Logger::log(Logger::Level::Info, "closed socket communication, socket: " + std::to_string(newSocket));
            //close(newSocket);

            //m_avialbleTasks.push_back(index);
        }
        Logger::log(Logger::Level::Info, "stopping server");
        close(sockfd);



    } catch (std::system_error& e){
        e.what();
    }


}


void MyParallelServer::acceptClient(ClientHandle::ClientHandler & c, int & socket) {
    
    Logger::log(Logger::Level::Info, "accepted connection");
    Logger::log(Logger::Level::Info, "open socket communication " + std::to_string(socket));
    char buf[1024];
    int bytesRead;


    bool running = true;
    bool didGetAnyInput = false;

    while (running) {
        std::stringstream ins;
        auto start = std::chrono::system_clock::now();
        Logger::log(Logger::Level::Debug, "start reading message");

        while (true) {
            bytesRead = recv(socket, buf, sizeof(buf) - 1, MSG_DONTWAIT);
            int readerrno = errno;
            bool wouldblock = ((bytesRead < 0) && (readerrno == EWOULDBLOCK));

            if (bytesRead > 0){
                buf[bytesRead] = 0;
                std::string msg = buf;
                Logger::log(Logger::Level::Debug, "recived:" + msg);
                didGetAnyInput = true;
            }

            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end-start;
            int seconds = diff.count() ;

            if ((seconds > 5) && (didGetAnyInput == false)) {
                Logger::log(Logger::Level::Info, "5 seconds without any input - closing the connection");
                running = false;
                break;
            }
            if (seconds > 30) {
                Logger::log(Logger::Level::Info, "timeout reading - didnt find valid message");
                running = false;
                break;
            }

            if (wouldblock){
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                continue;
            }
            if (bytesRead == 0) {
                Logger::log(Logger::Level::Info, "closed by peer");
                running = false;
                break;
            }

            if (bytesRead < 0) {  
                Logger::log(Logger::Level::Error, "exception while reading");
                running = false;
                break;
            }

            if ((size_t)ins.tellp() > maxMsgSize - sizeof(buf)) {
                Logger::log(Logger::Level::Error, "message too big" + std::to_string((size_t)ins.tellp()));
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
                Logger::log(Logger::Level::Error, "exception while writing");
                break;
            }
            if (byteWriten == 0) {
                break;
            }
            Logger::log(Logger::Level::Info, "writing:  " + outs.str());
            outs.seekp(byteWriten, std::ios_base::cur);
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end-start;

            int seconds = diff.count();
            if (seconds > 20) {
                Logger::log(Logger::Level::Info, "timeout writing");
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