/*
 * ECEE 2160 Homework assignment 1 matrix definitions.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-13
 * Due:     2020-07-17
 *
 * References
 * ==========
 *
 *  - https://en.cppreference.com/w/cpp/header/stdexcept
 *
 */

#include "matrix.h"

#include <iostream>     // for i/o definitions
#include <stdexcept>    // for std::logic_error
#include <utility>      // for std::swap

/**
 * Delimiter for print matrices.
 *
 * Defined using C-style string instead of a string view since the compiler
 * on the COE machines do not support C++17.
 */
constexpr const char DELIM[] = "\t\t";

IntMatrix::IntMatrix(std::size_t rows, std::size_t cols)
    : m_rows{rows},
      m_cols{cols},
      // We allow the allocation of zero sized arrays and assume that
      // (rows * cols) < std::numeric_limits<std::size_t>::max()
      m_values{new IntMatrix::Elem[rows * cols]} {}

IntMatrix::~IntMatrix()
{
    delete[] m_values;
    m_values = nullptr;
}

IntMatrix::Elem& IntMatrix::operator[](IntMatrix::Index elem_index)
{
    const auto row = elem_index.first;
    const auto col = elem_index.second;

    if (row >= m_rows || col >= m_cols) {
        throw std::out_of_range("invalid matrix index");
    }
    return m_values[row * m_cols + col];
}

void IntMatrix::index_transpose_square()
{
    // Transpose is only defined for square matrices in this assignment.
    if (m_cols != m_rows) {
        throw std::logic_error("matrix must be square");
    }

    // Iterator over the indices of the lower-triangular portion of the matrix
    //
    // For a 3x3 matrix, we only need to perform 3 swaps.
    for (std::size_t i_row{1}; i_row < m_rows; ++i_row) {
        for (std::size_t i_col{0}; i_col < i_row; ++i_col) {
            std::swap(
                // Call IntMatrix::operator[] to perform array indexing.
                (*this)[{i_row, i_col}],
                (*this)[{i_col, i_row}]
            );
        }
    }
}

void IntMatrix::pointer_transpose_square()
{
    // Transpose is only defined for square matrices in this assignment.
    if (m_cols != m_rows) {
        throw std::logic_error("matrix must be square");
    }

    // Iterator over the pointer offsets corresponding to the lower-triangular
    // portion of the matrix
    for (std::size_t i_row{1}; i_row < m_rows; ++i_row) {
        for (std::size_t i_col{0}; i_col < i_row; ++i_col) {
            std::swap(
                *(m_values + (i_row * m_cols + i_col)),
                *(m_values + (i_col * m_cols + i_row))
            );
        }
    }
}

std::ostream& operator<<(std::ostream& out, const IntMatrix& mat)
{
    std::size_t col_counter{0};
    for (const auto elem : mat) {
        // Cycle column counter
        col_counter = (col_counter + 1) % mat.m_cols;
        // If this is the last column of a row, print a newline.
        out << elem << (col_counter == 0 ? "\n" : DELIM);
    }
    return out;
}

std::istream& operator>>(std::istream& in, IntMatrix& mat)
{
    for (auto& elem : mat) {
        in >> elem;
    }
    return in;
}


