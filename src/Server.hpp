#pragma once
#include "ClientHandler.hpp"
#include <thread>

namespace ServerSide {

//interface
class Server {
public:
    static const size_t backlogSize = 20;
    Server();
    void virtual open(int port, ClientHandle::ClientHandlerGenerator& c) = 0;//change the void
    void virtual stop() = 0;
    void virtual acceptClient(ClientHandle::ClientHandler & c, int & socket) = 0;


private:
    uint16_t m_port;

};

class MyParallelServer : public Server {
public:
    MyParallelServer();
    //opening the socket and operates the clientHandler
    void virtual open(int port, ClientHandle::ClientHandlerGenerator& c);
    //stopping the communication
    void virtual stop();

    void acceptClient(ClientHandle::ClientHandler & c, int & socket);

private:
    std::vector<size_t> m_avialbleThreads;
    std::vector<std::shared_ptr<ClientHandle::ClientHandler>> m_clientHandlers;
    std::vector<int> m_clientSockets;

    constexpr static size_t threadPoolSize = 20;
    constexpr static size_t maxMsgSize = 10000;



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