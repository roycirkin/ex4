#pragma once
#include <sstream>
#include "Solver.hpp"
#include "algorithm"


namespace ClientHandle {

class ClientHandler {

void virtual handleClient (std::stringstream& inputStream, std::stringstream& outputSTream) = 0;

};

//inteface
class AlgorithmClientHandler  : public  ClientHandler{


};


class GraphHandler : public AlgorithmClientHandler{
public:
    GraphHandler(){}
    void handleClient (std::stringstream& inputStream, std::stringstream& outputSTream) {
         std::ignore = inputStream;
         std::ignore = outputSTream;

    }
    void findGraphPath();

};




}

