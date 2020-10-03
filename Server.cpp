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


namespace ServerSide {

const int successes = 0;

Server::Server() : m_pClientHandler(nullptr),  m_port(0) {}

void MyParallelServer::open(int port, ClientHandle::ClientHandler & c){
    try {
        setClientHandler(&c);
        setPort(port);

        int opt = 1;
        size_t const maxMsgSize = 10000;
        std::ignore = maxMsgSize;
        auto sockfd = socket(AF_INET, SOCK_STREAM, 0);
        Logger::log(Logger::Level::Info, "opening the server");
        if (sockfd < 0) {
            Logger::log(Logger::Level::Error, "cant open the socket");
            throw std::system_error{errno, std::system_category()};
        }

        sockaddr_in address;
        address.sin_family = AF_INET; 
        address.sin_addr.s_addr = INADDR_ANY; 
        address.sin_port = htons( port );

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) ///////////what???
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

        while (true) {
            newSocket = accept(sockfd, &pear_address, &addrlen);
            if (newSocket < 0) { 
                Logger::log(Logger::Level::Error, "connection refused");
                throw std::system_error{errno, std::system_category()};
            }

            Logger::log(Logger::Level::Info, "accepted connection");
            Logger::log(Logger::Level::Info, "open socket communication " + std::to_string(newSocket));
            char buf[1024];
            int bytesRead;

            // struct timeval tv;
            // tv.tv_sec = 0;
            // tv.tv_usec = 100;
            //setsockopt(newSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
            //int ddd = 0;/////


            bool running = true;
            while (running) {
                std::stringstream ins;
                auto start = std::chrono::system_clock::now();
                Logger::log(Logger::Level::Debug, "start reading message");

                while (true) {
                    bytesRead = recv(newSocket, buf, sizeof(buf) - 1, MSG_DONTWAIT);
                    int readerrno = errno;
                    bool wouldblock = ((bytesRead < 0) && (readerrno == EWOULDBLOCK));

                    if (bytesRead > 0){
                        buf[bytesRead] = 0;
                        std::string msg = buf;
                        Logger::log(Logger::Level::Debug, "recived:" + msg);
                    }

                    auto end = std::chrono::system_clock::now();
                    std::chrono::duration<double> diff = end-start;
                    int seconds = diff.count() ;

                    if (seconds > 20) {
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
                    if (getPClientHandler()->validateMsg(ins) == successes) {
                        break;
                    }

                }
                if (!running) {
                    break;
                }

                std::stringstream outs;
                if (getPClientHandler()->handleClient(ins, outs) != ClientHandle::GraphSolverStatus::successes) {
                    running = false;
                    break;
                }

                Logger::log(Logger::Level::Info, "returned message is: " + outs.str());

                int byteWriten = 0;
                start = std::chrono::system_clock::now();

                while (outs) {
                    byteWriten = write(newSocket, outs.str().c_str(), outs.str().length());
                    if (byteWriten < 0) {
                        Logger::log(Logger::Level::Error, "exception while writing");
                        throw std::system_error{errno, std::system_category()};
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
            }
        }
        Logger::log(Logger::Level::Info, "closed socket communication" + std::to_string(newSocket));
        close(newSocket);

    } catch (std::system_error& e){
        e.what();
    }


}


void  MyParallelServer::stop() {

}
MyParallelServer::MyParallelServer() {

}

uint16_t Server::getPort() const {
    return m_port;
}
ClientHandle::ClientHandler* Server::getPClientHandler() {
    return m_pClientHandler;
}
void Server::setPort (uint16_t port) {
    m_port = port;
}
void Server::setClientHandler (ClientHandle::ClientHandler* c) {
    m_pClientHandler = c;
}




}