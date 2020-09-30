#include "Server.hpp"
#include <errno.h>
#include <system_error>
#include <arpa/inet.h>
#include <sys/socket.h>


namespace ServerSide {

Server::Server() : m_pClientHandler(nullptr),  m_port(0) {}

void MyParallelServer::open(int port, ClientHandle::ClientHandler & c){
    setClientHandler(&c);
    setPort(port);
    auto sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw std::system_error{errno, std::system_category()};
    }
    sockaddr_in connectAddress;
    
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