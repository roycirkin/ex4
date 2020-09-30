#pragma once
#include <sstream>
#include "Solver.hpp"


namespace ClientHandle {

class ClientHandler {

void virtual handleClient (std::stringstream& inputStream, std::stringstream& outputSTream) = 0;

};

//inteface
class AlgorithmClientHandler  : public  ClientHandler{


};

template <class T>
class GraphHandler : public AlgorithmClientHandler{

Solver::GraphSolver<T> solver;

void findGraphPath(Graphs::Graph& g, size_t start, size_t end);

};




}

