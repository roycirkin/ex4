#pragma once
#include "ClassMatrix.hpp"
#include <string>
#include <vector>

namespace MatrixParsering {

void getMatrixFromVector(std::vector<double>& matVector, matrix::Matrix& theMatrix);

matrix::Matrix& getMatrixFromFile(const std::string& path);

void printMatrixToFile(matrix::Matrix& theMatrix, const std::string& outputFilePath);

}