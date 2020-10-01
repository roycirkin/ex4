#pragma once
#include "ClientHandler.hpp"

namespace ServerSide {

//interface
class Server {
public:
    static const size_t backlogSize = 20;
    Server();
    void virtual open(int port, ClientHandle::ClientHandler& c) = 0;//change the void
    void virtual stop() = 0;
    uint16_t getPort() const;
    ClientHandle::ClientHandler* getPClientHandler() ;
    void setPort (uint16_t port);
    void setClientHandler (ClientHandle::ClientHandler* c);


private:
    ClientHandle::ClientHandler *m_pClientHandler;
    uint16_t m_port;

};

class MyParallelServer : public Server {
public:
    MyParallelServer();
    //opening the socket and operates the clientHandler
    void virtual open(int port, ClientHandle::ClientHandler& c);
    //stopping the communication
    void virtual stop();



};

class UnSuportedServerError{
public:
    void print() {
        std::cerr << "the program doesnt support such server";
    }
};
class UnknownPortError{
public:
    void print() {
        std::cerr << "unkown port";
    }
};


























}