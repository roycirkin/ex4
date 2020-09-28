#include "Logger.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    argc++;
    Logger::startLog(std::cerr);
    Logger::log(Logger::Level::Info, "hello "  + std::string(argv[0]));


    return 0;
}









