#include "Server.hpp"
#include <errno.h>
#include <system_error>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include "Logger.hpp"
#include <sstream>


namespace ServerSide {

Server::Server() : m_pClientHandler(nullptr),  m_port(0) {}

void MyParallelServer::open(int port, ClientHandle::ClientHandler & c){
    setClientHandler(&c);
    setPort(port);

    int opt = 1;
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
    Logger::log(Logger::Level::Info, "waiting for someone to connect");

    int addrlen = sizeof(address);
    int newSocket = 0;
    if ((newSocket = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)<0)) { 
        Logger::log(Logger::Level::Error, "connection refused");
        throw std::system_error{errno, std::system_category()};
    }

    struct timeval tv;
    tv.tv_sec = 10; // timeout_in_seconds;
    tv.tv_usec = 0;
    setsockopt(newSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    Logger::log(Logger::Level::Info, "accepted connection");
    char buf[1024];
    ssize_t recived = recv(newSocket, &buf, 1024, 0);

    //std::stringstream ss;
    //ss.write(buf, recived);


    
    //Logger::log(Logger::Level::Debug, "recived :" + ss.str());





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