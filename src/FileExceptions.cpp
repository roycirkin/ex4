#include "FileExceptions.hpp"
#include <iostream>


namespace FileExceptions{
    void OpenFileExceptionForReading::printException() {
        std::cout << "Error: The file in a bad state!(can not open the file for reading)" << std::endl;
    }

    void ReadFileException::printException() {
        std::cout << "Error: Error occurred while reading the file!" << std::endl;
    }

    void OpenFileExceptionForWriting::printException() {
        std::cout << "Error: The bmp file in a bad state!(can not open bmp file for writing)" << std::endl;
    }

    void WriteFileException::printException() {
        std::cout << "Error: Error occurred while writing to file!" << std::endl;
    }

    void FileNotBmpException::printException() {
        std::cout << "Error: The file is not a bmp file!" << std::endl;
    }

    void NULLPointerException::printException() {
        std::cout << "Error: your object is null!" << std::endl;
    }

    void WrongBMPFileException::printException() {
        std::cout << "Error: The file is not a BMP file! " << std::endl;
    }
    
    void FileExceptions::printException() {
        std::cout << "Error: cant read or write from the file!" << std::endl;
    }
    void WrongMatrixFileException::printException() {
        std::cout << "Error: cant make this file a matrix" << std::endl;
    }

}