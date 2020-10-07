#include "MatrixExceptions.hpp"
extern "C" {
#include "ErrorCode.h"
}
#include <iostream>

namespace Exceptions {

void ErrorExceedMatrix::printException(){
    std::cout << "ERROR: The column or row index given exceed the matrix boundaries!" << std::endl;
}


void ErrorCantAdd::printException(){
    std::cout << "ERROR: The matrix sizes are not equal and therefore it is not possible to perform an add operation" << std::endl;
}

void ErrorCantMult::printException(){
    std::cout << "ERROR: The number of rows in the left matrix is not equal to the number of columns in the right matrix and therefore it is not possible to perform a multiplication operation" << std::endl;
}

void ErrorNullMatrix::printException(){
     std::cout << "ERROR: The matrix that given is null!" << std::endl;
}

void ErrorCantAllocateMatrix::printException(){
    std::cout << "Error: The memory allocation has failed" << std::endl;
}

void ErrorMatrixSizeNotAppopriate::printException(){
    std::cout << "Error: The height or width given in create matrix function to the matrix is 0!" << std::endl;
}

/**
 * @brief The function throw appopriate error according error code
 * 
 * @param code - code to ErrorCode
 */
void throwAppopriateErrorIfHas(ErrorCode code){
	switch ((int)code) {
		case EXCEED_MATRIX_SIZE_ERROR:
			throw ErrorExceedMatrix();
		case MATRIX_ADD_ERROR:
			throw ErrorCantAdd();
		case MATRIX_MULT_ERROR:
			throw ErrorCantMult();
		case NULL_ERROR:
			throw ErrorNullMatrix();
		case OUT_OF_MEMORY_ERROR:
			throw ErrorCantAllocateMatrix();
		case MATRIX_SIZE_NOT_APPOPRIATE:
			throw ErrorMatrixSizeNotAppopriate();
		}
	}
}