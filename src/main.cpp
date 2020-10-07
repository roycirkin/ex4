#include "Logger.hpp"
#include "Graph.hpp"
#include "matrix/ClassMatrix.hpp"
#include "Solver.hpp"
#include "Server.hpp"
#include "SearchAlgo.hpp"
#include "ClientHandler.hpp"
#include <signal.h>
#include <iostream>
#include <vector>
#include <errno.h>
#include <system_error>
#include <thread>
#include <csignal>

namespace boot {
class Main{
private:
    ServerSide::MyParallelServer m_server;
public:
    void run(int argc, char * argv[]) {

        try {
            uint16_t port = 0;
            char server_type = 'p';

            //reading the port from the command line
            if ((argc <= 1) || (!(port = std::atoi(argv[1])))) {
                Logger::log(Logger::Level::Error, "wrong port input ");
                throw ServerSide::UnknownPortError();
            }

            //checks what kind of server we operate
            if (argc > 2) {
                if (std::string(argv[2])== std::string("parallel")) {
                    server_type = 'p';
                } else if (std::string(argv[2])== std::string("serial")) {
                    throw ServerSide::UnSuportedServerError();
                } else {
                    Logger::log(Logger::Level::Error, "unknown server type");
                    throw ServerSide::UnSuportedServerError();
                }
            }

            ClientHandle::GraphHandlerGenerator c;
            if (server_type == 'p') {
                m_server.open(port, c);
            }
            Logger::log(Logger::Level::Error, "server finished his run");


        } catch (ServerSide::UnSuportedServerError e){
            e.print();
        } catch (ServerSide::UnknownPortError e){
            e.print();
        }

    }

    void signalHandler()
    {
    }

    void stop()
    {
        m_server.stop();
    }
};


}

boot::Main g_main = boot::Main();
// ^c is the key to stop
static void sig_handler(int signal)
{
    std::ignore = signal;
    std::cerr << "recived signal " << signal << std::endl;
    g_main.stop();
    //exit(-1);
}

int main(int argc, char* argv[]) {
    Logger::startLog(std::cerr);
    std::signal(SIGINT, sig_handler);
    std::signal(SIGTERM, sig_handler);
    std::signal(SIGQUIT, sig_handler);
    std::signal(SIGKILL, sig_handler);

    g_main.run(argc, argv);

    return 0;
}





