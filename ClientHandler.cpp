#include "ClientHandler.hpp"
#include "Logger.hpp"
#include <regex>


namespace ClientHandle {

GraphHandler::GraphHandler() : m_stage(hello),  m_algo(ASTAR) {}

GraphSolverStatus GraphHandler::handleClient (std::stringstream& inputStream, std::stringstream& outputStream) {

    switch (m_stage) {
    case hello:
        return parseHello(inputStream, outputStream);
        break;
    
    default:
        return failed;
        break;
    }




}

GraphSolverStatus GraphHandler::parseHello(std::stringstream& inputStream, std::stringstream& outputStream) {

    Logger::log(Logger::Level::Debug, "the input is:       \n"+inputStream.str());

    std::string solve;
    std::string problem;
    std::string algorithm;
    inputStream >> solve >> problem;

    std::regex algoDFS("(( |\t)*)DFS(( |\t)*)\\r\\n\\r\\n");
    std::regex algoBFS("(( |\t)*)BFS(( |\t)*)\\r\\n\\r\\n");
    std::regex algoASTAR("(( |\t)*)A\\*(( |\t)*)\\r\\n\\r\\n");
    std::regex blankLines("(( |\t)*)\\r\\n\\r\\n");
    std::smatch matcher;

    std::string instr = inputStream.str();
    std::transform(solve.begin(), solve.end(), solve.begin(), ::toupper);
    std::transform(problem.begin(), problem.end(), problem.begin(), ::toupper);
    std::transform(instr.begin(), instr.end(), instr.begin(), ::toupper);


    if (solve != "SOLVE") {
        return wrongMessage;
    }

    if (problem == "FIND-GRAPH-PATH") {
        if (std::regex_match(instr, matcher, algoBFS)) {
            m_algo = BFS;
        } else if (std::regex_match(instr, matcher, algoDFS)) {
            m_algo = DFS;
        } else if (std::regex_match(instr, matcher, algoASTAR)) {
            m_algo = ASTAR;
        } else if (std::regex_match(instr, matcher, blankLines)) {
            m_algo = ASTAR;
        } else {
        return wrongMessage;
        }
    }

    outputStream << "requestion accepted, please send the wanted matrix-graph";
    return successes;

}


}
