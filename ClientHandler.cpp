#include "ClientHandler.hpp"
#include "Logger.hpp"
#include "FileExceptions.hpp"
#include "matrix/ClassMatrix.hpp"
#include "matrix/MatrixParser.hpp"
#include <regex>
#include <string>
#include <iostream>


namespace ClientHandle {

stageInProtocolGraphHandler operator++(stageInProtocolGraphHandler& s) {
int temp = (int)s;
stageInProtocolGraphHandler new_s;
if (temp != (int)sendGraph) {
++temp;
new_s = (stageInProtocolGraphHandler)temp;
} else {
new_s = hello;
}
return new_s;
}

GraphHandler::GraphHandler() : m_stage(hello){

    searches.push_back(std::make_shared<Solver::GraphSolver<Algorithm::AstarAlgo>>());
    searches.push_back(std::make_shared<Solver::GraphSolver<Algorithm::BFSAlgo>>());
    searches.push_back(std::make_shared<Solver::GraphSolver<Algorithm::DFSAlgo>>());
    m_solver = searches.begin()->get();
    m_solver->getName();
}

bool GraphHandler::validateMsg(std::stringstream& inputstream) {
    switch (m_stage) {
        case hello:
            return validateHello(inputstream);
            break;
        case sendGraph:
            return validateSendGraph(inputstream);
            break;
        default:
            return failed;
            break;
    }


}

GraphSolverStatus GraphHandler::handleClient (std::stringstream& inputStream, std::stringstream& outputStream) {

    switch (m_stage) {
    case hello:
        return handleHello(inputStream, outputStream);
        break;
    // case sendGraph:
    //     return parseHello(inputStream, outputStream);
    //     break;
    default:
        return failed;
        break;
    }




}

bool GraphHandler::validateHello(std::stringstream& inputStream) {

    std::string solve;
    std::string problem;
    std::string algorithm;
    inputStream >> solve >> problem;

    std::smatch matcher;

    std::string instr = inputStream.str();
    std::transform(solve.begin(), solve.end(), solve.begin(), ::toupper);
    std::transform(problem.begin(), problem.end(), problem.begin(), ::toupper);
    std::transform(instr.begin(), instr.end(), instr.begin(), ::toupper);

    if (solve != "SOLVE") {
        return false;
        Logger::log(Logger::Level::Debug, "solve failed - " + solve);
    }

    if (problem == "FIND-GRAPH-PATH") {

        std::vector<std::shared_ptr<Solver::Solver>>::iterator it;

        for (it = searches.begin(); it != searches.end(); ++it) {

            std::string forTheRegex = "SOLVE(( |\t)*)FIND-GRAPH-PATH(( |\t)*)" + (it)->get()->getName() + "(( |\t)*)\r\n\r\n";
            if ((it)->get()->getName() == "A*") {
                forTheRegex = "SOLVE(( |\t)*)FIND-GRAPH-PATH(( |\t)*)A\\*(( |\t)*)\r\n\r\n";
            }
            std::regex searchAlgoRegex(forTheRegex);

            if (std::regex_match(instr, matcher, searchAlgoRegex)) {
                m_solver = it->get();
                return true;
            }
        }
        std::regex blankLines("SOLVE(( |\t)*)FIND-GRAPH-PATH(( |\t)*)\r\n\r\n");
        if (!std::regex_match(instr, matcher, blankLines)) {
            return false;
        }
    } else {
        return false;
    }
    
    return true;

}

bool GraphHandler::validateSendGraph(std::stringstream& inputStream) {

    std::string line;
    int height = 0;
    int width = 0;
    int startX = 0;
    int startY = 0;
    int endX = 0;
    int endY = 0;
    
    if (!std::getline(inputStream, line)) {// why does it returns me an empty string???????????? and if it brings me an empty string i want to enter this if!!!
                        Logger::log(Logger::Level::Debug, "1");
        return false;
    }
    std::cout << inputStream.str();
    if  (!getTwoNumbersInALine(line, height, width)) {
                        Logger::log(Logger::Level::Debug, "2");
        return false;
    }
    if ((height == 0) || (width == 0)) {
                         Logger::log(Logger::Level::Debug, "3");
        return false;
    }
    std::cout << "benzemaaa" << " " << height << width <<std::endl;

    std::fstream file;
    std::string hashMatrix = std::to_string(getHash(inputStream.str()));
    file.open("matrix" + hashMatrix, std::ios::out|std::ios::trunc);
    if (file.fail()) {
        throw FileExceptions::OpenFileExceptionForReading();
                 Logger::log(Logger::Level::Debug, "4");

    }

    for (int i = 0; i < height; ++i) {
        if (!std::getline(inputStream, line)) {
            return false;
        }
        file.write(line.c_str(), line.length());
    }
    file.close();

    try { 
        matrix::Matrix mat = MatrixParsering::getMatrixFromFile("matrix" + hashMatrix);
        if ((int)mat.matrixGetWidth() != width) {
            return false;
        }
    } catch (...) {
        return false;
    }
    
    if (!std::getline(inputStream, line)) {
        return false;
    }
    if  (!getTwoNumbersInALine(line, startX, startY)) {
        return false;
    }
    if (!std::getline(inputStream, line)) {
        return false;
    }
    if  (!getTwoNumbersInALine(line, endX, endY)) {
        return false;
    }
    if ((!(isPointInMatrix(height, width, startX, startY))) || (!(isPointInMatrix(height, width, endX, endY)))) {
        return false;
    }


    return true;
    
    





}


GraphSolverStatus GraphHandler::handleHello(std::stringstream& inputStream, std::stringstream& outputStream) {
    Logger::log(Logger::Level::Debug, "in client handler - hello");
    std::ignore = inputStream;
    outputStream << "requestion accepted";
    m_stage = sendGraph;
    return successes;
}

size_t getHash(const std::string& str) {
    std::hash<std::string> hashfn;
    auto hash = hashfn(str);
    return hash;
}

bool getTwoNumbersInALine(std::string& line, int& a, int&b) {
            std::cout << "line:" <<line;

    std::string num("[[:digit:]]+");
    std::regex numRGX(num);
    std::regex heightAndWidth("(( |\t)*)" + num + "(( |\t)*),(( |\t)*)" + num + "(( |\t)*)");
    std::smatch matcher;
    int numTemp = 0;

    if(!std::regex_match(line, matcher, heightAndWidth)) {
        return false;
    }

    std::regex_search(line, matcher, numRGX);
    std::string temp = matcher.str(0);
    try {numTemp = std ::stoi(temp);}
    catch (...){return false;}
    a = numTemp;
    line = line.substr(temp.length(), line.length());
    std::regex_search(line, matcher, numRGX);
    temp = matcher.str(0);
    try{numTemp = std ::stoi(temp);}
    catch (...){return false;}
    b = numTemp;

    return true;
}

bool isPointInMatrix(size_t height, size_t width, int posX, int posY) {
    if ((posX < 0) || (posX >= (int)width) || (posY < 0) || (posY >= (int)height)) {
        return false;
    }
    return true;

}








}
