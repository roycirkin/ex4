#pragma once
#include "ClassMatrix.hpp"
#include <string>
#include <vector>

namespace MatrixParsering {

void getMatrixFromVector(std::vector<double>& matVector, matrix::Matrix& theMatrix);

const matrix::Matrix& getMatrixFromFile(const std::string& path);

void printMatrixToFile(matrix::Matrix& theMatrix, const std::string& outputFilePath);

bool getLine(std::string& input, std::string& line);

bool getLineFromFile(std::ifstream& path, std::string& line);

const matrix::Matrix& getMatrixFromString(std::string& content);

}