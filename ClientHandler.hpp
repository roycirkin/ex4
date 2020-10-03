#pragma once
#include <sstream>
#include <regex>
#include <string>
#include "Solver.hpp"
#include "algorithm"
#include "matrix/ClassMatrix.hpp"


namespace ClientHandle {

//the stages in the communication
enum stageInProtocolGraphHandler {
hello, sendGraph

};

enum GraphSolverAlgorithms {
BFS, DFS, ASTAR
};


enum GraphSolverStatus {
    successes, noPath, unsuportedTaskError, wrongMatrixGraphInput, wrongGraph, unknownErrorOccuerd, unvalidInput
};

class ClientHandler {
public:
    GraphSolverStatus virtual handleClient (std::stringstream& inputStream, std::stringstream& outputSTream) = 0;
    int virtual validateMsg(std::stringstream& inputstream) = 0;
    virtual std::string getSyncString() = 0;
    virtual void update() = 0;

};

//inteface
class AlgorithmClientHandler  : public  ClientHandler{


};


class GraphHandler : public AlgorithmClientHandler{
private:
    stageInProtocolGraphHandler m_stage;
    Solver::GraphSolveBase* m_solver;
    std::vector<std::shared_ptr<Solver::GraphSolveBase>> searches;
    //std::vector<std::string> algoNames;
    //std::string m_algoName;
public:
    GraphHandler();
    GraphSolverStatus virtual handleClient (std::stringstream& inputStream, std::stringstream& outputStream);
    int virtual validateMsg(std::stringstream& inputstream);

    int virtual validateHello(std::stringstream& inputstream);
    GraphSolverStatus handleHello(std::stringstream& inputStream, std::stringstream& outputStream);

    int validateSendGraph(std::stringstream& inputStream);
    GraphSolverStatus handleSendGraph(std::stringstream& inputStream, std::stringstream& outputStream);

    Solver::GraphSolveBase* getSolver();

    std::string getSyncString() {
        return "\r\n\r\n";
    }
    virtual void update();

};

class GraphSolverProtocolMsg {
private :
    enum GraphSolverStatus m_status;
public:
    //returns the protocol status message to the client - default
    void status_string(std::stringstream& ss) {
        ss << "Version: 1.0" << "\r\n" << "status: " << (int)getStatus() << "\r\n" << "response-length: ";
    }
    //returns the protocol status message to the client
    virtual std::string to_string(GraphHandler& graphHandler) = 0;
    GraphSolverProtocolMsg(enum GraphSolverStatus status) : m_status(status) {}
    enum GraphSolverStatus getStatus() {return m_status;}

};

class GraphSolverProtocolMsgHello : public GraphSolverProtocolMsg {
public:
    GraphSolverProtocolMsgHello(enum GraphSolverStatus status) : GraphSolverProtocolMsg(status) {}

    //the messgae the client gets in the hello stage
    virtual std::string to_string(GraphHandler& graphHandler);
};

class GraphSolverProtocolMsgsendGraph : public GraphSolverProtocolMsg {
public:
    GraphSolverProtocolMsgsendGraph(enum GraphSolverStatus status) : GraphSolverProtocolMsg(status) {}

    //the messgae the client gets in the hello stage
    virtual std::string to_string(GraphHandler& graphHandler);
    
};

GraphSolverStatus operator++(GraphSolverStatus s);

class unsuportedTaskError {
public:
    void print() {
        std::cerr << "this operation is unsuported by this clienr handler.";
    }
};


size_t getHash(const std::string& str);

//gets the numbers from a line which contains 2
bool getTwoNumbersInALine(std::string& line, int& a, int&b);

bool isPointInMatrix(size_t height, size_t width, int posX, int posY);


}

