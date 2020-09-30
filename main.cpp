#include "Logger.hpp"
#include "Graph.hpp"
#include "matrix/ClassMatrix.hpp"
#include "Solver.hpp"
#include "SearchAlgo.hpp"
#include <iostream>
#include <vector>
#include <errno.h>
#include <system_error>


namespace boot {
class Main{
public:
    void main(int argc, char * argv[]) {
        uint16_t port = 0;
        //uint8_t server_type = 'p';

        if ((argc <= 1) || (!(port = std::atoi(argv[1])))) {
            Logger::log(Logger::Level::Error, "wrong port input ");
            //throw
        }

        if (argc > 2) {
            if (std::string(argv[2])== std::string("parallel")) {
                //server_type = 'p';
            } else if (std::string(argv[2])== std::string("serial")) {
                //server_type = 's';
                Logger::log(Logger::Level::Error, "unsuported server");
                //throw
            } else {
                Logger::log(Logger::Level::Error, "unknown server type");
                //throw
            }
        }
        



    }
};

}

int main(int argc, char* argv[]) {
    Logger::startLog(std::cerr);
    boot::Main server = boot::Main();
    server.main(argc,argv);

    vector<double> costs = 
    {
        4,6,3,9,
    };

    vector<Graphs::Edge> edges =
	{
		{ 0, 1 }, { 0, 2 }, { 1, 0 }, { 1, 3 },
		{ 2, 0 }, { 2, 3 }, { 3, 1 }, { 3, 2},
	};

    Graphs::Graph graph(edges,costs);
    Graphs::printGraph(graph);
    std::cout << std::endl;

    matrix::Matrix m(4, 4);
    // m.matrixSetValue(0, 0, 5);
    // m.matrixSetValue(0, 1, 80);
    // m.matrixSetValue(0, 2, 12);
    // m.matrixSetValue(1, 0, 3);
    // m.matrixSetValue(1, 1, -1);
    // m.matrixSetValue(1, 2, -1);
    // m.matrixSetValue(2, 0, 1);
    // m.matrixSetValue(2, 1, 10);
    // m.matrixSetValue(2, 2, 10);
    // m.matrixSetValue(3, 0, 1);
    // m.matrixSetValue(3, 1, -1);
    // m.matrixSetValue(3, 2, 2);
    m.matrixSetValue(0, 0, 5);
    m.matrixSetValue(0, 1, 500);
    m.matrixSetValue(0, 2, 500);
    m.matrixSetValue(0, 3, 5);
    m.matrixSetValue(1, 0, 499.5);
    m.matrixSetValue(1, 1, 500);
    m.matrixSetValue(1, 2, 500);
    m.matrixSetValue(1, 3, -1);
    m.matrixSetValue(2, 0, 5);
    m.matrixSetValue(2, 1, 500);
    m.matrixSetValue(2, 2, 500);
    m.matrixSetValue(2, 3, 5);
    m.matrixSetValue(3, 0, 5);
    m.matrixSetValue(3, 1, 5);
    m.matrixSetValue(3, 2, 5);
    m.matrixSetValue(3, 3, 5);




    Graphs::Graph graph2(m);
    Graphs::printGraph(graph2);

    double price = 0;
    
    Solver::GraphSolver<Algorithm::DFSAlgo> solver(graph2, 0, 11);
    solver.solve();
    price = solver.getPrice();
    price++;
    price--;




    return 0;
}





