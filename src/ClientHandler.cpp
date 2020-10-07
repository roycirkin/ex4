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

}

Solver::GraphSolveBase* GraphHandler::getSolver() {
    return m_solver;
}


int GraphHandler::validateMsg(std::stringstream& inputstream) {
    int res = 0;
    switch (m_stage) {
        case hello:
            res =  validateHello(inputstream);
            break;
        case sendGraph:
            res =  validateSendGraph(inputstream);
            break;
        default:
            return unknownErrorOccuerd;
            break;
    }

    inputstream.seekg(std::ios_base::beg);
    return res;

}

GraphSolverStatus GraphHandler::handleClient (std::stringstream& inputStream, std::stringstream& outputStream) {

    switch (m_stage) {
    case hello:
        return handleHello(inputStream, outputStream);
        break;
    case sendGraph:
        return handleSendGraph(inputStream, outputStream);
        break;
    default:
        return unknownErrorOccuerd;
        break;
    }

}

int GraphHandler::validateHello(std::stringstream& inputStream) {

    std::string solve;
    std::string problem;
    std::string algorithm;
    inputStream >> solve >> problem;

    std::smatch matcher;

    std::string instr = inputStream.str();
    std::transform(solve.begin(), solve.end(), solve.begin(), ::toupper);
    std::transform(problem.begin(), problem.end(), problem.begin(), ::toupper);
    std::transform(instr.begin(), instr.end(), instr.begin(), ::toupper);


        std::vector<std::shared_ptr<Solver::GraphSolveBase>>::iterator it;

        //finds which algorithm will the solver use
        for (it = searches.begin(); it != searches.end(); ++it) {

            std::string forTheRegex = "SOLVE(( |\t)*)FIND-GRAPH-PATH(( |\t)*)" + (it)->get()->getName() + "(( |\t)*)\r\n\r\n";
            if ((it)->get()->getName() == "A*") {
                forTheRegex = "SOLVE(( |\t)*)FIND-GRAPH-PATH(( |\t)*)A\\*(( |\t)*)\r\n\r\n";
            }
            std::regex searchAlgoRegex(forTheRegex);

            if (std::regex_match(instr, matcher, searchAlgoRegex)) {
                m_solver = it->get();
                return successes;
            }
        }
        std::regex blankLines("SOLVE(( |\t)*)FIND-GRAPH-PATH(( |\t)*)\r\n\r\n");
        if (!std::regex_match(instr, matcher, blankLines)) {
            return unsuportedTaskError;
        }

    
    return successes;

}

int GraphHandler::validateSendGraph(std::stringstream& inputStream) {

    std::string inputString = inputStream.str();
    std::string line;
    int height = 0;
    int width = 0;
    int startX = 0;
    int startY = 0;
    int endX = 0;
    int endY = 0;

    //gets the width and the height
    if (!MatrixParsering::getLine(inputString, line)) {
        return wrongMatrixGraphInput;
    }
    if  (!getTwoNumbersInALine(line, height, width)) {
        return wrongMatrixGraphInput;
    }
    if ((height == 0) || (width == 0)) {
        return wrongMatrixGraphInput;
    }

    std::string downLine = "\r\n";
    std::string matrixString;
    for (int i = 0; i < height; ++i) {
        if (!MatrixParsering::getLine(inputString, line)) {
            return wrongMatrixGraphInput;
        }
        matrixString.append(line);
        matrixString.append(downLine);
    }

    size_t index = 0;
    //checks if you entered a negative number
    if (matrixString.find("-") != std::string::npos){
        return wrongMatrixGraphInput;
    }

    while (true) {
        // Locate the substring to replace. - replacing all the "b" with "-1"
        index = matrixString.find("b", index);
        if (index == std::string::npos) {
            break;
        }
        matrixString.replace(index, 1, "-1");

        // Advance index forward so the next iteration doesn't pick it up as well. 
        index += 2;
    }

    //creates the matrix
    try {
        std::shared_ptr<matrix::Matrix> mat = std::make_shared<matrix::Matrix>(MatrixParsering::getMatrixFromString(matrixString));
        if ((int)(mat.get())->matrixGetWidth() != width) {
            return wrongMatrixGraphInput;
        }
        Graphs::MatrixGraph g(*(mat.get()));
        (m_solver)->setGraph(g);
        
    }catch (FileExceptions::FileExceptions& e) {
        e.printException();
        return wrongMatrixGraphInput;
    }
    
    //gets the start point
    if (!MatrixParsering::getLine(inputString, line)) {
        return wrongMatrixGraphInput;
    }
    if  (!getTwoNumbersInALine(line, startY, startX)) {
        return wrongMatrixGraphInput;
    }

    //gets the end point
    if (!MatrixParsering::getLine(inputString, line)) {
        return wrongMatrixGraphInput;
    }
    if  (!getTwoNumbersInALine(line, endY, endX)) {
        return wrongMatrixGraphInput;
    }

    if ((!(isPointInMatrix(height, width, startX, startY))) || (!(isPointInMatrix(height, width, endX, endY)))) {
        return wrongMatrixGraphInput;
    }

    (m_solver)->setStart(width * startY + startX);
    (m_solver)->setEnd(width * endY + endX);///////////////////

    std::regex blankLines("\r\n");
    std::smatch matcher;
    if (!std::regex_match(inputString, matcher, blankLines)) {
        return wrongMatrixGraphInput;
    }
    

    return successes;


}


GraphSolverStatus GraphHandler::handleHello(std::stringstream& inputStream, std::stringstream& outputStream) {

    GraphSolverStatus res = successes;
    //if the input is not valid:
    auto valid = validateHello(inputStream);
    if (valid != successes) {
        res = GraphSolverStatus::unvalidInput;
    }

    std::ignore = inputStream;
    GraphSolverProtocolMsgHello msg(res);
    outputStream << msg.to_string(*this);

    return res;
}

GraphSolverStatus GraphHandler::handleSendGraph(std::stringstream& inputStream, std::stringstream& outputStream) {

    GraphSolverStatus res = successes;
    auto valid = validateSendGraph(inputStream);
    //if the input is not valid:
    if (valid != successes) {
        res = GraphSolverStatus::unvalidInput;
        GraphSolverProtocolMsgsendGraph msg(res);
        outputStream << msg.to_string(*this);
        return res;
    }

    std::ignore = inputStream;

    auto status = m_solver->solve();

    if (status == Solver::Status_solver::no_path) {

        GraphSolverProtocolMsgsendGraph msg(noPath);
        outputStream << msg.to_string(*this);
        return successes;

    } else if(status == Solver::Status_solver::success) {

        GraphSolverProtocolMsgsendGraph msg(successes);
        outputStream << msg.to_string(*this);
        return successes;

    } else if(status == Solver::Status_solver::no_path) {

        GraphSolverProtocolMsgsendGraph msg(noPath);
        outputStream << msg.to_string(*this);
        return noPath;

    } else if(status == Solver::Status_solver::wrong_graph) {

        GraphSolverProtocolMsgsendGraph msg(wrongGraph);
        outputStream << msg.to_string(*this);
        return wrongGraph;

    }
    return unknownErrorOccuerd;


    std::ignore = outputStream;
}


size_t getHash(const std::string& str) {
    std::hash<std::string> hashfn;
    auto hash = hashfn(str);
    return hash;
}

bool getTwoNumbersInALine(std::string& line, int& a, int&b) {

    std::string num("[[:digit:]]+");
    std::regex numRGX(num);
    std::regex twoNumRegex("(( |\t)*)" + num + "(( |\t)*),(( |\t)*)" + num + "(( |\t)*)");
    std::regex commaRegex("(( |\t)*)" + num + "(( |\t)*),");
    std::smatch matcher;
    int numTemp = 0;

    if(!std::regex_match(line, matcher, twoNumRegex)) {
        return false;
    }

    std::regex_search(line, matcher, numRGX);
    std::string temp = matcher.str(0);
    numTemp = std ::stoi(temp);
    a = numTemp;
    std::regex_search(line, matcher, commaRegex);
    temp = matcher.str(0);
    line = line.substr(temp.length() , line.length());
    std::regex_search(line, matcher, numRGX);
    temp = matcher.str(0);
    numTemp = std ::stoi(temp);
    b = numTemp;

    return true;
}

bool isPointInMatrix(size_t height, size_t width, int posX, int posY) {
    if ((posX < 0) || (posX >= (int)width) || (posY < 0) || (posY >= (int)height)) {
        return false;
    }
    return true;

}

    std::string GraphSolverProtocolMsgHello::to_string(GraphHandler& graphHandler) {
        std::ignore = graphHandler;
        std::stringstream ss;
        status_string(ss);
        ss << "0" << "\r\n\r\n";
        return ss.str();
    }

    std::string GraphSolverProtocolMsgsendGraph::to_string(GraphHandler& graphHandler) {
        
        std::stringstream ss;
        status_string(ss);
        if (getStatus() == successes) {

            std::string solution = std::to_string(graphHandler.getSolver()->getPrice()) + "," + graphHandler.getSolver()->getRoute();
            ss << solution.length() << "\r\n" << solution << "\r\n\r\n";

        } else if (getStatus() == noPath) {

            std::string noPathString = "no path found from the given start to the given end";
            ss << noPathString.length() << "\r\n" << noPathString << "\r\n\r\n";

        } else if (getStatus() == wrongGraph) {

            std::string wrongGraphString = "the given graph is unvalid";
            ss << wrongGraphString.length() << "\r\n" << wrongGraphString << "\r\n\r\n";

        } else if (getStatus() == unvalidInput) {
            ss << "0" << "\r\n\r\n";
        }
        return ss.str();
    }

    void GraphHandler::update(bool& run) {
        if (m_stage == sendGraph) {
            m_stage = hello;
            run = false;
            return;
        }
        if (m_stage == hello) {
            m_stage = sendGraph;
            return;
        }
    }


    std::shared_ptr<ClientHandler> GraphHandlerGenerator::generate() {
        return std::make_shared<GraphHandler>();
    }








}
