cmake_minimum_required(VERSION 3.0.0)
project(ex4.out VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
add_compile_options(-Wall -Wextra -pedantic -Werror)
enable_testing()

add_executable(ex4.out matrix/MatrixParser.cpp matrix/MatrixExceptions.cpp matrix/ErrorCode.c matrix/Matrix.c matrix/ClassMatrix.cpp  Graph.cpp SearchAlgo.cpp Solver.cpp Logger.cpp Server.cpp ClientHandler.hpp main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)