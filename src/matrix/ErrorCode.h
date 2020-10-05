#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    // DON'T CHANGE THIS VALUE!
    ERROR_SUCCESS = 0, EXCEED_MATRIX_SIZE_ERROR, MATRIX_ADD_ERROR,
     MATRIX_MULT_ERROR, NULL_ERROR, OUT_OF_MEMORY_ERROR, MATRIX_SIZE_NOT_APPOPRIATE,
} ErrorCode;

/**
 * @brief Checks if a given error code indicates a success or not.
 *
 * @param[in] code the error code.
 * @return whether the error code indicates a success or not.
 */
bool error_isSuccess(ErrorCode code);

/**
 * @brief gets a textual error message for a given error code.
 *
 * @param[in] code the error code.
 * @return const char* the textual representation of the error code.
 */
const char* error_getErrorMessage(ErrorCode code);