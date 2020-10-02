#pragma once
#include <sstream>
#include <regex>
#include "Solver.hpp"
#include "algorithm"
#include "matrix/ClassMatrix.hpp"


namespace ClientHandle {

enum stageInProtocolGraphHandler {
hello, sendGraph

};
enum GraphSolverAlgorithms {
BFS, DFS, ASTAR
};

enum GraphSolverStatus {
successes, failed,
};

GraphSolverStatus operator++(GraphSolverStatus s);


class ClientHandler {
public:
    GraphSolverStatus virtual handleClient (std::stringstream& inputStream, std::stringstream& outputSTream) = 0;
    bool virtual validateMsg(std::stringstream& inputstream) = 0;
    bool virtual validateHello(std::stringstream& inputstream) = 0;


};

//inteface
class AlgorithmClientHandler  : public  ClientHandler{


};


class GraphHandler : public AlgorithmClientHandler{
private:
    stageInProtocolGraphHandler m_stage;
    Solver::Solver* m_solver;
    std::vector<std::shared_ptr<Solver::Solver>> searches;
public:
    GraphHandler();
    GraphSolverStatus virtual handleClient (std::stringstream& inputStream, std::stringstream& outputStream);
    bool virtual validateMsg(std::stringstream& inputstream);
    bool virtual validateHello(std::stringstream& inputstream);
    GraphSolverStatus handleHello(std::stringstream& inputStream, std::stringstream& outputStream);

    bool validateSendGraph(std::stringstream& inputStream);
    GraphSolverStatus handleSendGraph(std::stringstream& inputStream, std::stringstream& outputStream);

    //GraphSolverStatus parseSendGraph(std::stringstream& inputStream, std::stringstream& outputStream);
    //void findGraphPath();

};


class unsuportedTaskError {
public:
    void print() {
        std::cerr << "this operation is unsuported by this clienr handler.";
    }
};


size_t getHash(const std::string& str);

bool getTwoNumbersInALine(std::string& line, int& a, int&b);

bool isPointInMatrix(size_t height, size_t width, int posX, int posY);

}

