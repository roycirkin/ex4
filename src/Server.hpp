#pragma once
#include "ClientHandler.hpp"
#include <thread>
#include <atomic>

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

    void clientThreadFunc();

private:
    std::vector<size_t> m_avialbleTasks;
    std::vector<size_t> m_taskQueue;
    std::mutex m_taskQueueMutex;
    std::mutex m_avialbleTasksMutex;
    std::vector<std::shared_ptr<ClientHandle::ClientHandler>> m_clientHandlers;
    std::vector<int> m_clientSockets;
    std::atomic<bool> m_running;
    std::mutex m_serverMutex;
    std::vector<std::thread> m_threads;

    constexpr static const size_t threadPoolSize = 20;
    constexpr static const size_t maxMsgSize = 10000;
    constexpr static const size_t timeoutNoReading = 5;
    constexpr static const size_t noValidMessageTimeout = 30;
    std::condition_variable m_cv;
    std::mutex m_conditionVarMutex;



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