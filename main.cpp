#include "Logger.hpp"
#include "Graph.hpp"
#include "matrix/ClassMatrix.hpp"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    argc++;
    Logger::startLog(std::cerr);
    Logger::log(Logger::Level::Info, "hello "  + std::string(argv[0]));

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

    matrix::Matrix m(2,2);
    m.matrixSetValue(0, 0, 4);
    m.matrixSetValue(0, 1, 6);
    m.matrixSetValue(1, 0, 3);
    m.matrixSetValue(1, 1, 9);

    Graphs::Graph graph2(m);
    Graphs::printGraph(graph2);







    return 0;
}









