#include "MatrixParser.hpp"
#include "../FileExceptions.hpp"
#include <fstream>
#include <regex>
#include <iostream>
#include <math.h>
#include <system_error>


namespace MatrixParsering {

const u_int8_t MaxToRead = 100;

matrix::Matrix getMatrixFromString(std::string& content) {

    std::string line;
    std::regex validity("((( |\t)*)(-?)[[:digit:]]+((\\.[[:digit:]]+)?)(( |\t)*)(,))*(( |\t)*)(-?)[[:digit:]]+((\\.[[:digit:]]+)?)(( |\t)*)");    
    std::regex rgx("(( |\t)*)(-?)[[:digit:]]+((\\.[[:digit:]]+)?)(( |\t)*)(,?)");    
    std::regex rgxNum("(-?)[[:digit:]]+((\\.[[:digit:]]+)?)");    
    std::regex dot("((\\.[[:digit:]]+)).");  
    size_t lines = 0;
    size_t width = 0;
    std::vector<double> matrixVector;


    while(getLine(content, line)) {
        std::smatch validityMatcher;
        if(!std::regex_match(line, validityMatcher, validity)) {
            throw FileExceptions::WrongMatrixFileException();
        }

        std::smatch matcher;
        std::smatch numMatcher;
        std::smatch dotFinder;

        size_t varsPerLineCounter = 0;

        while(std::regex_search(line, matcher, rgx)) {
            std::string temp = matcher.str(0);
            std::regex_search(temp, numMatcher, rgxNum);
            double num = std ::stoi(numMatcher.str(0), nullptr);

            if (std::regex_search(temp, dotFinder, dot)) {
                std::string decimalTemp = dotFinder.str(0);
                std::regex_search(decimalTemp, numMatcher, rgxNum);
                double decialNum = std ::stoi(numMatcher.str(0), nullptr);
                size_t numOfZeros = 0;
                size_t iter = 1;
                while (decimalTemp[iter] == '0') {
                    numOfZeros++;
                    iter++;
                }
                while (decialNum >= 1 / pow (10, numOfZeros)) {
                    decialNum = decialNum / 10;
                }

                if (num >= 0) {
                    num += decialNum;
                }
                if (num < 0) {
                    num -= decialNum;
                }
            }

            matrixVector.push_back(num);
            line = line.substr(temp.length(), line.length());
            varsPerLineCounter++;
        }
        if (lines == 0) {
            width = varsPerLineCounter;
        }
        if (varsPerLineCounter != width) {
            throw FileExceptions::WrongMatrixFileException();
        }

        lines++;
    }

    matrix::Matrix mat(lines,width);

    getMatrixFromVector(matrixVector, mat);

    return mat;
}

const matrix::Matrix getMatrixFromFile(const std::string& path) {
    std::ifstream file;         
    file.open(path, std::ios::in); 
    if (file.fail()) {
        throw FileExceptions::OpenFileExceptionForReading();
    }
    std::stringstream ss;
    ss << file.rdbuf();
    std::string fileContent = ss.str();
    file.close();
    return getMatrixFromString(fileContent);

}


void getMatrixFromVector(std::vector<double>& matVector, matrix::Matrix& theMatrix) {

    if (matVector.size() != theMatrix.matrixGetWidth() * theMatrix.matrixGetHeight()) {
        //throw invalid vector or matrix
    }

    for (size_t i = 0;i < theMatrix.matrixGetHeight(); ++i) {
        for (size_t j = 0; j < theMatrix.matrixGetWidth(); ++j) {
            theMatrix.matrixSetValue(i, j, matVector[i * theMatrix.matrixGetWidth() + j]);
        }
    }
}


bool getLineFromFile(std::ifstream& path, std::string& line) {
std::stringstream ss;
ss << path.rdbuf();
std::string content = ss.str();
return getLine(content, line);

}

bool getLine(std::string& input, std::string& line) {
    std::regex lineRegex("[^\r\n]+");
    std::smatch matcher;
    std::string downLine = "\r\n";

    if (!std::regex_search(input, matcher, lineRegex)) {
        return false;
    }
    line = matcher.str(0);
    input = input.substr(line.length() + downLine.length(), input.length());
    if (line == ""){
        return false;
    }
    return true;
}




}