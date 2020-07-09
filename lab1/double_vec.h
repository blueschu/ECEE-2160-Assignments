/*
 * ECEE 2160 Lab Assignment 1 vector declarations.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-08
 *
 */

#ifndef ECEE_2160_LAB_REPORTS_DOUBLE_VEC_H
#define ECEE_2160_LAB_REPORTS_DOUBLE_VEC_H

#include <cstddef>
#include <optional>

/**
 * Vector implementation specialized for doubles.
 *
 * Non-goals in this implementation:
 *
 *  - Move semantics
 *  - Exception safety
 */
class DoubleVec {

    /// Data type for vector elements.
    using Elem = double;

    /// Default value for a vector's size.
    constexpr inline static std::size_t DEFAULT_SIZE{2};

    /**
     * The elements stored by this vector
     */
    Elem* m_values;

    /**
     * The number of elements that can be held in the currently allocated
     * memory.
     *
     * This member variable is analogous to `std::vector::capacity`.
     */
    std::size_t m_size;

    /**
     * The number of elements currents stored in this vector.
     *
     * This member variable is analogous to `std::vector::size`.
     */
    std::size_t m_count;

  public:

    /// Type for mutable iterators for this vector.
    using iterator = Elem*;

    /// Type for immutable iterators for this vector.
    using const_iterator = Elem*;

    // Default constructor
    explicit DoubleVec(std::size_t size = DEFAULT_SIZE);

    // Destructor.
    //
    // Not declared virtual since inheritance is not expected.
    ~DoubleVec();

    /**
     * Returns the number of elements that can be held in the currently
     * allocated memory.
     */
    size_t size() const
    {
        return m_size;
    }

    /**
     * Returns the number of elements currently stored in this vector.
     */
    size_t count() const
    {
        return m_count;
    }

    /**
     * Adds the given element to the end of this vector.
     *
     * This function reallocates this vectors storage if the current storage
     * is full.
     * @param elem Element to be added.
     */
    void append(Elem elem);

    /**
     * Removes the last element of this vector.
     *
     * This function mimics the behavior of the the std::Vec:pop function
     * from the Rust standard library.
     *
     * https://doc.rust-lang.org/std/vec/struct.Vec.html#method.pop
     *
     * @return The last element, if it exists.
     */
    std::optional<Elem> pop();

    /**
     * Inserts the given element at the the specified index in this vector.
     *
     * This function reallocates this vectors storage if the current storage
     * is full
     *
     * @param index Location to insert the element.
     * @param elem Element to be added.
     */
    void insert(std::size_t index, Elem elem);

    /*
     * Move semantics were out of the scope of this lab.
     *
     * To prevent accidents, we explicitly disallow the compiler from generating
     * definitions for {copy,move} {constructors, assignment operators}.
     */
    DoubleVec(const DoubleVec&) = delete;

    DoubleVec(DoubleVec&&) = delete;

    DoubleVec& operator=(const DoubleVec&) = delete;

    DoubleVec& operator=(DoubleVec&&) = delete;

    /*
     * Iterator protocol definitions.
     */
    iterator begin() { return m_values; }

    iterator end() { return m_values + m_count; }

    const_iterator begin() const { return m_values; }

    const_iterator end() const { return m_values + m_count; }

  private:
    /// Returns the size that this vector should have when it grows.
    std::size_t next_size() const
    {
        // Make sure the the size at least meets the default size.
        return std::max(DEFAULT_SIZE, m_size * 2);
    }

    /**
     * Increases the storage allocated by this vector.
     */
    void grow();

    /**
     * Reallocates this vector's storage to reduce its size.
     *
     * This function is guaranteed not to remove elements when shrinking the
     * storage.
     */
    void shrink();

};

#endif //ECEE_2160_LAB_REPORTS_DOUBLE_VEC_H
