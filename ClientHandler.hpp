#pragma once
#include <sstream>
#include <regex>
#include <string>
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
    successes, failed, unsuportedTaskError, wrongMatrixGraphInput,
};


class GraphSolverProtocolMsg {
private :
    enum GraphSolverStatus m_status;
protected:
    void status_string(std::stringstream& ss) {
        ss << "Version: 1.0" << std::endl << "status: " << (int)getStatus() << std::endl << "response-length: ";
    }
public:
    virtual std::string to_string() = 0;
    GraphSolverProtocolMsg(enum GraphSolverStatus status) : m_status(status) {}
    enum GraphSolverStatus getStatus() {return m_status;}

};

class GraphSolverProtocolMsgHello : public GraphSolverProtocolMsg {
public:
    GraphSolverProtocolMsgHello(enum GraphSolverStatus status) : GraphSolverProtocolMsg(status) {}

    virtual std::string to_string() {
        std::stringstream ss;
        status_string(ss);
        if (getStatus() == successes) {
            ss << "0";
        } else if (getStatus() == unsuportedTaskError) {
            std::string unsuportedOperation = "unsuported operation given";
            ss << unsuportedOperation.length() << unsuportedOperation;
        }
        return ss.str();
    }
};

class GraphSolverProtocolMsgsendGraph : public GraphSolverProtocolMsg {
public:
    GraphSolverProtocolMsgsendGraph(enum GraphSolverStatus status) : GraphSolverProtocolMsg(status) {}
    
    virtual std::string to_string() {
        return "";

    }
    
};

GraphSolverStatus operator++(GraphSolverStatus s);

class ClientHandler {
public:
    GraphSolverStatus virtual handleClient (std::stringstream& inputStream, std::stringstream& outputSTream) = 0;
    int virtual validateMsg(std::stringstream& inputstream) = 0;
};

//inteface
class AlgorithmClientHandler  : public  ClientHandler{


};


class GraphHandler : public AlgorithmClientHandler{
private:
    stageInProtocolGraphHandler m_stage;
    Solver::Solver* m_solver;
    std::vector<std::shared_ptr<Solver::Solver>> searches;
    std::string m_algoName;
public:
    GraphHandler();
    GraphSolverStatus virtual handleClient (std::stringstream& inputStream, std::stringstream& outputStream);
    int virtual validateMsg(std::stringstream& inputstream);
    int virtual validateHello(std::stringstream& inputstream);
    GraphSolverStatus handleHello(std::stringstream& inputStream, std::stringstream& outputStream);

    int validateSendGraph(std::stringstream& inputStream);
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

