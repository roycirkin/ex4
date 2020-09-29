extern "C" {
#include "Matrix.h"
}
#include "MatrixExceptions.hpp"
#include "MatrixParser.hpp"
#include "ClassMatrix.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>


namespace matrix{


    void WrongFileError::printError() {
        std::cout << "cant create a matrix from this file";
    }

    Matrix::Matrix(const uint32_t height, const uint32_t width) :m_width(width), m_height(height){
        Exceptions::throwAppopriateErrorIfHas(matrix_create(&this->m_pMatrix, m_height, m_width));
    }

    Matrix::Matrix(const Matrix& matrix) :m_width(matrix.m_width), m_height(matrix.m_height){
        Exceptions::throwAppopriateErrorIfHas(matrix_copy(&this->m_pMatrix, matrix.m_pMatrix));
    }

    Matrix::Matrix(Matrix&& matrix) :m_width(matrix.m_width), m_height(matrix.m_height){
        this->m_pMatrix = matrix.m_pMatrix;
        matrix.m_pMatrix = nullptr;
    }

    Matrix::Matrix(const std::string& path) {
        try {
            MatrixParsering::getMatrixFromFile(path);
        } catch (...) {
            throw WrongFileError();
        }
    }    

    void Matrix::setMatrixFromVector(std::vector<double> vec) {
        MatrixParsering::getMatrixFromVector(vec, *this);
    }

    void Matrix::printToFile(const std::string& path) {
        MatrixParsering::printMatrixToFile(*this, path);
    }

    Matrix& Matrix::operator=(const Matrix& other){
        if(this != &other){
            matrix_destroy(this->m_pMatrix);
            *this = Matrix(other);
        }
        return *this;
    }

    Matrix& Matrix::operator=(Matrix&& other){
         if(this != &other){
            matrix_destroy(this->m_pMatrix);
            this->m_pMatrix = other.m_pMatrix;
            this->m_height = other.m_height;
            this->m_width = other.m_width;
            other.m_pMatrix = nullptr;
        }
        return *this;
    }

    void Matrix::matrixSetValue(const uint32_t rowIndex, const uint32_t colIndex, const double value){
        Exceptions::throwAppopriateErrorIfHas(matrix_setValue(this->m_pMatrix, rowIndex, colIndex, value));
    }

    double Matrix::matrixgetValue(const uint32_t rowIndex, const uint32_t colIndex) const{
        double value = 0;
        Exceptions::throwAppopriateErrorIfHas(matrix_getValue(this->m_pMatrix, rowIndex, colIndex, &value));
        return value;
    }
    
    uint32_t Matrix::matrixGetHeight() const{
        uint32_t result;
        Exceptions::throwAppopriateErrorIfHas(matrix_getHeight(this->m_pMatrix, &result));
        return result;
    } 

    uint32_t Matrix::matrixGetWidth() const{
        uint32_t result;
        Exceptions::throwAppopriateErrorIfHas(matrix_getWidth(this->m_pMatrix, &result));
        return result;
    }
    
    double Matrix::operator()(const uint32_t rowIndex, const uint32_t colIndex) const{
        double value = 0;
        Exceptions::throwAppopriateErrorIfHas(matrix_getValue(this->m_pMatrix, rowIndex, colIndex, &value));
        return value;
    }


    Matrix Matrix::operator+(const Matrix& matrix) const{
        Matrix result(matrix.m_height, matrix.m_width);
        if(matrix.m_height == 0 || matrix.m_width == 0){
            throw Exceptions::ErrorMatrixSizeNotAppopriate();
        }
        Exceptions::throwAppopriateErrorIfHas(matrix_add(&result.m_pMatrix, this->m_pMatrix, matrix.m_pMatrix));
        return result;
    }

    Matrix Matrix::operator*(const Matrix& matrix) const{
        Matrix result(matrix.m_height, this->m_height);
        if(matrix.m_height == 0 || this->m_height == 0){
            throw Exceptions::ErrorMatrixSizeNotAppopriate();
        }
        Exceptions::throwAppopriateErrorIfHas(matrix_multiplyMatrices(&result.m_pMatrix, this->m_pMatrix, matrix.m_pMatrix));
        return result; 
    }

    Matrix Matrix::operator*(const double scalar) const{
        Matrix result(*this);
        Exceptions::throwAppopriateErrorIfHas(matrix_multiplyWithScalar(result.m_pMatrix, scalar));
        return result;
    }

    Matrix::~Matrix(){
        matrix_destroy(this->m_pMatrix);
    }
        
    Matrix operator*(double scalar, Matrix& matrix){
        return matrix * scalar;
    }  

    // std::vector<uint8_t> Matrix::touint8() {
        
    //     // width 4 + hight + 4 + 8 *  width * hight
    //     size_t serialzieSize = sizeof(m_pMatrix->width) + sizeof(m_pMatrix->height) + m_pMatrix->height * m_pMatrix->width * sizeof(double);
    //     std::vector<uint8_t> serializeData(serialzieSize);

    //     *((size_t*)&serializeData[0]) = m_pMatrix->height;
    //     *((size_t*)&serializeData[sizeof(m_pMatrix->height)]) = m_pMatrix->width;
    //     size_t bytesMatrixArray = m_pMatrix->height * m_pMatrix->width * sizeof(double);
    //     memcpy(&serializeData[2 * sizeof(m_pMatrix->height)], m_pMatrix->ptr, bytesMatrixArray);
    //     return serializeData;
    // }
}

std::ostream& operator<<(std::ostream& os, const matrix::Matrix& m) {
    for (size_t row = 0; row < m.matrixGetHeight(); ++row) {
        for (size_t col = 0; col < m.matrixGetWidth(); ++col) {
            os << m.matrixgetValue(row, col);
            if (col< m.matrixGetWidth() - 1) {
                os << ',';
            }
        }
        os << std::endl;
    }
    return os;
}