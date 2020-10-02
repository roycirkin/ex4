#pragma once
#include <sstream>
#include "Solver.hpp"
#include "algorithm"


namespace ClientHandle {

enum stageInProtocolGraphHandler {
hello,

};
enum GraphSolverAlgorithms {
BFS, DFS, ASTAR
};

enum GraphSolverStatus {
successes, failed, wrongMessage,
};

class ClientHandler {
public:
    GraphSolverStatus virtual handleClient (std::stringstream& inputStream, std::stringstream& outputSTream) = 0;
    bool virtual validateMsg(std::stringstream& inputstream) = 0;

};

//inteface
class AlgorithmClientHandler  : public  ClientHandler{


};


class GraphHandler : public AlgorithmClientHandler{
private:
    stageInProtocolGraphHandler m_stage;
    GraphSolverAlgorithms m_algo;
public:
    GraphHandler();
    GraphSolverStatus virtual handleClient (std::stringstream& inputStream, std::stringstream& outputStream);
    bool virtual validateMsg(std::stringstream& inputstream) {
        std::ignore = inputstream;
        return true;
    }
    GraphSolverStatus parseHello(std::stringstream& inputStream, std::stringstream& outputStream);
    void findGraphPath();

};


class unsuportedTaskError {
public:
    void print() {
        std::cerr << "this operation is unsuported by this clienr handler.";
    }
};




}

