#include "MatrixParser.hpp"
#include "../FileExceptions.hpp"
#include <fstream>
#include <regex>
#include <iostream>
#include <math.h>
#include <system_error>


namespace MatrixParsering {

const u_int8_t MaxToRead = 100;

matrix::Matrix& getMatrixFromFile(const std::string& path) {

    std::ifstream file;
    file.open(path, std::ios::in);
    if (file.fail()) {
        throw FileExceptions::OpenFileExceptionForReading();
    }
    std::string line;

    std::regex validity("(( *)(-?)[[:digit:]]+((\\.[[:digit:]]+)?)( *)(,))*( *)(-?)[[:digit:]]+((\\.[[:digit:]]+)?)( *)");
    std::regex rgx("( *)(-?)[[:digit:]]+((\\.[[:digit:]]+)?)( *)(,?)");
    std::regex rgxNum("(-?)[[:digit:]]+((\\.[[:digit:]]+)?)");
    std::regex dot("((\\.[[:digit:]]+)).");
    size_t lines = 0;
    size_t width = 0;
    std::vector<double> matrixVector;

    while(std::getline(file, line)) {
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

    file.close();

    matrix::Matrix* mat = new matrix::Matrix(lines,width);

    getMatrixFromVector(matrixVector, *mat);
    


    return *mat;
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

// void printMatrixToFile( matrix::Matrix& theMatrix, const std::string& outputFilePath) {

//     std::ofstream outputFile;
//     outputFile.open(outputFilePath, std::ios::out|std::ios::trunc);
//     if (!outputFile.is_open()) {
//        //throw app exception
//     }   

//     for (size_t i = 0;i < theMatrix.matrixGetHeight(); ++i) {
//         for (size_t j = 0;j < theMatrix.matrixGetWidth(); ++j) {
//             double data = theMatrix.matrixgetValue(i,j);
//             outputFile<<data;
//             if (j != theMatrix.matrixGetHeight() - 1) {
//                 outputFile<<',';
//             }
//         }
//         outputFile<<std::endl;
//     }
//     outputFile<<std::flush;
// }





// void parseMatrixGivenSize(int height, int width, std::stringstream) {

// std::string line;

//     std::regex validity("(( *)(-?)[[:digit:]]+((\\.[[:digit:]]+)?)( *)(,))*( *)(-?)[[:digit:]]+((\\.[[:digit:]]+)?)( *)");
//     std::regex rgx("( *)(-?)[[:digit:]]+((\\.[[:digit:]]+)?)( *)(,?)");
//     std::regex rgxNum("(-?)[[:digit:]]+((\\.[[:digit:]]+)?)");
//     std::regex dot("((\\.[[:digit:]]+)).");
//     size_t lines = 0;
//     size_t width = 0;
//     std::vector<double> matrixVector;

//     while(std::getline(file, line)) {
//         std::smatch validityMatcher;
//         if(!std::regex_match(line, validityMatcher, validity)) {
//             throw std::system_error{errno, std::system_category()};
//         }

//         std::smatch matcher;
//         std::smatch numMatcher;
//         std::smatch dotFinder;

//         size_t varsPerLineCounter = 0;

//         while(std::regex_search(line, matcher, rgx)) {
//             std::string temp = matcher.str(0);
//             std::regex_search(temp, numMatcher, rgxNum);
//             double num = std ::stoi(numMatcher.str(0), nullptr);

//             if (std::regex_search(temp, dotFinder, dot)) {
//                 std::string decimalTemp = dotFinder.str(0);
//                 std::regex_search(decimalTemp, numMatcher, rgxNum);
//                 double decialNum = std ::stoi(numMatcher.str(0), nullptr);
//                 size_t numOfZeros = 0;
//                 size_t iter = 1;
//                 while (decimalTemp[iter] == '0') {
//                     numOfZeros++;
//                     iter++;
//                 }
//                 while (decialNum >= 1 / pow (10, numOfZeros)) {
//                     decialNum = decialNum / 10;
//                 }

//                 if (num >= 0) {
//                     num += decialNum;
//                 }
//                 if (num < 0) {
//                     num -= decialNum;
//                 }
//             }

//             matrixVector.push_back(num);
//             line = line.substr(temp.length(), line.length());
//             varsPerLineCounter++;
//         }
//         if (lines == 0) {
//             width = varsPerLineCounter;
//         }
//         if (varsPerLineCounter != width) {
//             //throw //unvalid matrix;
//         }

//         lines++;
//     }

//     file.close();

//     matrix::Matrix* mat = new matrix::Matrix(lines,width);

//     getMatrixFromVector(matrixVector, *mat);
    


//     return *mat;


// }















}