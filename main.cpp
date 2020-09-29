#include "Logger.hpp"
#include "Graph.hpp"
#include "matrix/ClassMatrix.hpp"
#include "Solver.hpp"
#include "SearchAlgo.hpp"
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

    matrix::Matrix m(4, 3);
    m.matrixSetValue(0, 0, 5);
    m.matrixSetValue(0, 1, 80);
    m.matrixSetValue(0, 2, 12);
    m.matrixSetValue(1, 0, 3);
    m.matrixSetValue(1, 1, -1);
    m.matrixSetValue(1, 2, -1);
    m.matrixSetValue(2, 0, 1);
    m.matrixSetValue(2, 1, 10);
    m.matrixSetValue(2, 2, 10);
    m.matrixSetValue(3, 0, 1);
    m.matrixSetValue(3, 1, 4);
    m.matrixSetValue(3, 2, 2);


    Graphs::Graph graph2(m);
    Graphs::printGraph(graph2);

    size_t price = 0;
    Algorithm::BFSAlgo bfs;
    Algorithm::DFSAlgo dfs;
    Solver::GraphSolver solver(graph2, 0, 11, &bfs);
    solver.solve(& price);





    return 0;
}









