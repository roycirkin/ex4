#pragma once
#include <string>
#include <fstream>

namespace MatrixOperation {

enum matrixOperation {
    add, multiply,
};

void matOperation(const std::string& inputFile1, const std::string& inputFile2,const std::string& outputFile,
cacheManager::CacheManager& cache, matrixOperation m);



}
