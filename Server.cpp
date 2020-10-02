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

Server::Server() : m_pClientHandler(nullptr),  m_port(0) {}

void MyParallelServer::open(int port, ClientHandle::ClientHandler & c){
    setClientHandler(&c);
    setPort(port);

    int opt = 1;
    size_t const maxMsgSize = 1000;
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


        bool running = true;
        while (running) {
            std::stringstream ins;
            auto start = std::chrono::system_clock::now();
            Logger::log(Logger::Level::Debug, "start readin message");

            while (true) {
                bytesRead = recv(newSocket, buf, sizeof(buf), MSG_DONTWAIT);
                int readerrno = errno;
                bool wouldblock = ((bytesRead<0) && (readerrno == EWOULDBLOCK));

                auto end = std::chrono::system_clock::now();
                std::chrono::duration<double> diff = end-start;
                int seconds = diff.count() ;
                if (seconds > 20) {
                    Logger::log(Logger::Level::Info, "timeout reading");
                    running = false;
                    break;
                }

                if (wouldblock){
                    Logger::log(Logger::Level::Debug, "lalala "+std::to_string(bytesRead)+ " bytes");
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                    continue;
                }

                Logger::log(Logger::Level::Debug, "red "+std::to_string(bytesRead)+ " bytes");///////////////

                if (bytesRead == 0) {
                    Logger::log(Logger::Level::Info, "closed by peer");
                }

                if (bytesRead < 0) {  
                    Logger::log(Logger::Level::Error, "exception while reading");
                    throw std::system_error{errno, std::system_category()};
                }

                ins.write(buf, bytesRead);
                if ((size_t)ins.tellg() > maxMsgSize) {
                    break;
                }

                if (getPClientHandler()->validateMsg(ins)) {
                    Logger::log(Logger::Level::Info, "recived message");
                    break;  
                } 

            }
            if (!running) {
                break;
            }
            Logger::log(Logger::Level::Info, "message is: " + ins.str());

            std::stringstream outs;
            getPClientHandler()->handleClient(ins, outs);

            int byteWriten = 0;
            start = std::chrono::system_clock::now();

            while (outs) {
                byteWriten = write(newSocket, outs.str().c_str(), outs.str().length());
                if (byteWriten < 0) {
                    Logger::log(Logger::Level::Error, "exception while writing");
                    throw std::system_error{errno, std::system_category()};
                }

                outs.seekg(byteWriten, std::ios_base::cur);
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
        Logger::log(Logger::Level::Info, "closed socket communication" + std::to_string(newSocket));
        close(newSocket);

        /*std::stringstream ss;
        bool spaceFlag = false;
        for(char c : buf){
            if (!spaceFlag) {
                ss << c;
            }
            if (c == ' ') {
                spaceFlag = true;
            } else {
                spaceFlag = false;
            }
        }
        std::string requestion = ss.str();
        std::transform(requestion.begin(), requestion.end(), requestion.begin(), ::toupper);

        std::regex algorithmHandlerRegex("solve ");
        std::regex graphSolverRegex("find-graph-path ");
        size_t algorithmHandlerRegexSize = std::string("solve ").length();
        size_t graphSolverRegexSize = std::string("find-graph-path ").length();
        std::smatch matcher;

        if (std::regex_match(requestion.substr(0, algorithmHandlerRegexSize), matcher, algorithmHandlerRegex)) {
            if (std::regex_match(requestion.substr(algorithmHandlerRegexSize, graphSolverRegexSize),
             matcher, graphSolverRegex)) {
               if (requestion.substr(algorithmHandlerRegexSize + graphSolverRegexSize))
            }
        }*/





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