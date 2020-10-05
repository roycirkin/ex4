#pragma once
extern "C" {
#include "Matrix.h"
}
#include <stdint.h>
#include <vector>
#include <fstream>


namespace matrix{

    class WrongFileError {
    public:
        void printError();
    };

    class Matrix{ 
        
    public:
        /**
         * @brief Construct a new Matrix object
         * 
         * @param heightMatrix - the height of matrix
         * @param widthMatrix - the widht of matrix
         */
        Matrix(const uint32_t height, const uint32_t width);

        /**
         * @brief Copy Constructor 
         * 
         * @param matrix - matrix to copy
         */
        Matrix(const Matrix& matrix);

         /**
         * @brief Moov Constructor
         * 
         * @param matrix - matrix to copy
         */
        Matrix(Matrix&& matrix);

        Matrix(const std::string& path);

        void setMatrixFromVector(std::vector<double> vec);

        void printToFile(const std::string& path);

        std::string& toString(std::string& instream);

        /**
         * @brief The function copy a maatrix
         * 
         * @param other  - matrix to copy
         * @return Matrix& - new matrix that copied
         */
        Matrix& operator=(const Matrix& other);

        /**
         * @brief The move a matrix
         * 
         * @param other - matrix to move
         * @return Matrix& - matrix that moved
         */
        Matrix& operator=(Matrix&& other);
        
        /**
         * @brief Returns the height of a give matrix.
         * 
         * @return uint32_t - height of a give matrix.
         */
        uint32_t matrixGetHeight() const;

        /**
         * @brief Returns the width of a give matrix.
         * 
         * @return uint32_t - width of a give matrix.
         */
        uint32_t matrixGetWidth() const;

        /**
         * @brief Sets a value to the matrix.
         * 
         * @param rowIndex - Row index of the value to set.
         * @param colIndex - Column index of the value to setF.
         * @param value - value to set.
         */
        void matrixSetValue(const uint32_t rowIndex, const uint32_t colIndex, const double value);

        /**
         * @brief Get a value of cell of the matrix.
         * 
         * @param rowIndex -  Row index of the value to get.
         * @param colIndex - Column index of the value to get.
         * @return double - return get value.
         */
        double operator()(const uint32_t rowIndex, const uint32_t colIndex) const;

        /**
         * @brief The function added two matrixes.
         * 
         * @param matrix - matrix.
         * @return Matrix - return matrix.
         */
       Matrix operator+(const Matrix& matrix) const;

        /**
         * @brief Multiply operation between two matrixes.
         * 
         * @param matrix - matrix to multiply.
         * @return Matrix - new matrix of the result.
         */
        Matrix operator*(const Matrix& matrix) const;

        /**
         * @brief Operation that multiply scalar and matrix.
         * 
         * @param scalar - scalar to multiply.
         * @return Matrix - matrix of the result.
         */
        Matrix operator*(const double scalar) const;

        /**
         * @brief Destroy the Matrix object
         * 
         */
        ~Matrix();

        double matrixgetValue(const uint32_t rowIndex, const uint32_t colIndex) const;

        std::vector<uint8_t> touint8();

    private:
        uint32_t m_width;
        uint32_t m_height;
        PMatrix m_pMatrix;
    };
    
    /**
     * @brief The function multiply matrix and scalar.
     * 
     * @param scalar - scalar to multiply.
     * @param matrix - matrix to multiply.
     * @return Matrix - return matrix of the result.
     */
    Matrix operator*(double scalar, Matrix& matrix);
}

std::ostream& operator<<(std::ostream& os, const matrix::Matrix& m);