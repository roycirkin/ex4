#pragma once
extern "C" {
#include "ErrorCode.h"
}
namespace Exceptions {

class ErrorExceedMatrix {
public:
    /**
     * @brief The functoin print exception message of exeed matrix
     * 
     */
    void printException();
};

class ErrorCantAdd {
public:
    /**
     * @brief The function print exception message of add operation
     * 
     */
    void printException();
};

class ErrorCantMult {
public:
    /**
     * @brief The function print exception message of exeed matrix
     * 
     */
    void printException();
};

class ErrorNullMatrix {    
public:
    /**
     * @brief The functoin print null pointer error
     * 
     */
    void printException();
};

class ErrorCantAllocateMatrix {
public:
   /**
     * @brief The functoin allocation memory error error
     * 
     */
    void printException();
};

class ErrorMatrixSizeNotAppopriate {
public:
    /**
     * @brief The functoin print matrix sizes equall to 0 error
     * 
     */
    void printException();
};

/**
 * @brief The function throw appopriate error according error code
 * 
 * @param code - code to ErrorCode
 */
void throwAppopriateErrorIfHas(ErrorCode code);
}