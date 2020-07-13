/*
 * ECEE 2160 Homework assignment 1 matrix declarations.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-13
 * Due:     2020-07-17
 *
 */

#ifndef ECEE_2160_HOMEWORK_MATRIX_H
#define ECEE_2160_HOMEWORK_MATRIX_H

#include <cstdint>      // for std::size_t
#include <iosfwd>       // for std::ostream (no definitions)
#include <utility>      // for std::pair
#include <vector>       // for std::vector

/**
 * A matrix with integer entries.
 *
 * We implement manual dynamic memory allocations to store matrix entries per
 * the assignment instructions.
 */
class IntMatrix {
    /// Data type for matrix entries.
    using Elem = int;

    /// The number of rows in this matrix
    std::size_t m_rows;

    /// The number of columns per row.
    std::size_t m_cols;

    /**
     * The entries of this matrix
     *
     * We use a 1D representation of values stored in this matrix to simplify
     * memory management.
     */
    Elem* m_values;

  public:
    /// Data type for indexing a matrix via operator[].
    using Index = std::pair<std::size_t, std::size_t>;

    /// Type for mutable iterators for this matrix.
    using iterator = Elem*;

    /// Type for immutable iterators for this matrix.
    using const_iterator = const Elem*;

    IntMatrix(std::size_t rows, std::size_t cols);

    ~IntMatrix();

    /**
     * Computes the transpose of this matrix in-place using array-index syntax.
     *
     * This function raises std::logic_error is the matrix is not square.
     */
    void index_transpose_square();

    /**
    * Computes the transpose of this matrix in-place using manual pointer
     * arithmetic.
     *
     * This function raises std::logic_error is the matrix is not square.
    */
    void pointer_transpose_square();

    /**
     * Returns the number of rows in the matrix.
     */
    std::size_t rows() const { return m_rows; }

    /**
     * Returns the number of rows in the matrix.
     */
    std::size_t cols() const { return m_cols; }

    /*
     * Move semantics were out of the scope of this lab.
     *
     * To prevent accidents, we explicitly disallow the compiler from generating
     * definitions for {copy,move} {constructors, assignment operators}.
     */
    IntMatrix(const IntMatrix&) = delete;

    IntMatrix(IntMatrix&&) = delete;

    IntMatrix& operator=(const IntMatrix&) = delete;

    IntMatrix& operator=(IntMatrix&&) = delete;

    /*
     * Custom indexing operator to simplify translation of 2D indexes to the
     * 1D representation used by this matrix.
     */
    Elem& operator[](Index elem_index);

    /*
     * I/O stream operators.
     */
    friend std::ostream& operator<<(std::ostream& out, const IntMatrix& mat);

    friend std::istream& operator>>(std::istream& in, IntMatrix& mat);

    /*
    * Iterator protocol definitions.
     *
     * Mutable iterators are used by operator>>(std::istream&, IntMatrix&).
     * Immutable iterators are used by operator<<(std::ostream&, const IntMatrix&).
    */
    iterator begin() { return m_values; }

    iterator end() { return m_values + (m_rows * m_cols); }

    const_iterator begin() const { return m_values; }

    const_iterator end() const { return m_values + (m_rows * m_cols); }
};

#endif //ECEE_2160_HOMEWORK_MATRIX_H
