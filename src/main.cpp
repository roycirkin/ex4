#include "Logger.hpp"
#include "Graph.hpp"
#include "matrix/ClassMatrix.hpp"
#include "Solver.hpp"
#include "Server.hpp"
#include "SearchAlgo.hpp"
#include "ClientHandler.hpp"
#include <iostream>
#include <vector>
#include <errno.h>
#include <system_error>


namespace boot {
class Main{
public:
    void main(int argc, char * argv[]) {
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

            ServerSide::MyParallelServer server;
            ClientHandle::GraphHandlerGenerator c;
            if (server_type == 'p') {
                server.open(port, c);
            }       

        } catch (ServerSide::UnSuportedServerError e){
            e.print();
        } catch (ServerSide::UnknownPortError e){
            e.print();
        }

    }
};

}

int main(int argc, char* argv[]) {
    Logger::startLog(std::cerr);
    boot::Main server = boot::Main();
    server.main(argc,argv);

    // vector<double> costs = 
    // {
    //     4,6,3,9,
    // };

    // vector<Graphs::Edge> edges =
	// {
	// 	{ 0, 1 }, { 0, 2 }, { 1, 0 }, { 1, 3 },
	// 	{ 2, 0 }, { 2, 3 }, { 3, 1 }, { 3, 2},
	// };

    // Graphs::Graph graph(edges,costs);
    // Graphs::printGraph(graph);
    // std::cout << std::endl;

    // matrix::Matrix m(4, 4);
    // // m.matrixSetValue(0, 0, 5);
    // // m.matrixSetValue(0, 1, 80);
    // // m.matrixSetValue(0, 2, 12);
    // // m.matrixSetValue(1, 0, 3);
    // // m.matrixSetValue(1, 1, -1);
    // // m.matrixSetValue(1, 2, -1);
    // // m.matrixSetValue(2, 0, 1);
    // // m.matrixSetValue(2, 1, 10);
    // // m.matrixSetValue(2, 2, 10);
    // // m.matrixSetValue(3, 0, 1);
    // // m.matrixSetValue(3, 1, -1);
    // // m.matrixSetValue(3, 2, 2);
    // m.matrixSetValue(0, 0, 5);
    // m.matrixSetValue(0, 1, 500);
    // m.matrixSetValue(0, 2, 500);
    // m.matrixSetValue(0, 3, 5);
    // m.matrixSetValue(1, 0, 499.5);
    // m.matrixSetValue(1, 1, 500);
    // m.matrixSetValue(1, 2, 500);
    // m.matrixSetValue(1, 3, -1);
    // m.matrixSetValue(2, 0, 5);
    // m.matrixSetValue(2, 1, 5000);
    // m.matrixSetValue(2, 2, 500);
    // m.matrixSetValue(2, 3, 5);
    // m.matrixSetValue(3, 0, 5);
    // m.matrixSetValue(3, 1, 5);
    // m.matrixSetValue(3, 2, 5);
    // m.matrixSetValue(3, 3, 5);




    // Graphs::MatrixGraph graph2(m);
    // Graphs::printGraph(graph2);

    // double price = 0;
    
    // Solver::GraphSolver<Algorithm::AstarAlgo> solver(graph2, 0, 11);
    // solver.solve();
    // price = solver.getPrice();
    // price++;
    // price--;




    return 0;
}





