/*
 * ECEE 2160 Lab Assignment 1 vector implementation.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-08
 *
 */

// https://en.cppreference.com/w/cpp/utility/optional
// https://stackoverflow.com/questions/1952972/does-stdcopy-handle-overlapping-ranges

#include "double_vec.h"

#include <algorithm>

/**
 * Ratio between vector size and vector capacity at which the vector should
 * be shrunk.
 *
 * Threshold specified in lab instructions.
 */
constexpr double SHRINK_THRESHOLD{0.3};

/**
 * The maximum percent of the vector's capacity that can be removed by a single
 * call to shrink.
 */
constexpr double SHRINK_MAX_DECREASE{0.5};

DoubleVec::DoubleVec(std::size_t size)
    : m_size{size},
      m_count{0},
      m_values{size ? new DoubleVec::Elem[size]{} : nullptr}
{

}

DoubleVec::~DoubleVec()
{
    delete[] m_values;
    m_values = nullptr;
    m_size = 0;
    m_count = 0;
}

void DoubleVec::grow()
{
    const size_t new_size = next_size();
    auto* const new_values = new DoubleVec::Elem[new_size];

    std::copy(begin(), end(), new_values);

    delete[] m_values;
    m_values = new_values;
    m_size = new_size;
}

void DoubleVec::append(DoubleVec::Elem elem)
{
    if (m_count + 1 > m_size) {
        grow();
    }

    m_values[m_count] = elem;
    ++m_count;
}

std::optional<DoubleVec::Elem> DoubleVec::pop()
{
    // Check if there is an element to pop.
    std::optional<DoubleVec::Elem> result = m_count > 0
        // Get last element and decrease count.
        ? std::make_optional(m_values[--m_count])
        // Return empty value sentinel.
        : std::nullopt;

    // Check if the element count has reached the shrinking threshold.
    if (static_cast<double>(m_count) / m_size <= SHRINK_THRESHOLD) {
        shrink();
    }

    return result;
}

void DoubleVec::insert(std::size_t index, DoubleVec::Elem elem)
{
    if (index > m_size) {
        // Behavior for inserting at indices outside of element count isn't
        // defined in the lab instructions.
        throw std::invalid_argument("index cannot exceed vector length");
    }

    if (m_count + 1 > m_size) {
        grow();
    }

    // Shift all elements at and to the right of `index`.
    //
    // copy_backwards only requires that the end of the output range does not
    // overlap with the input range, so this copy is safe.
    std::copy_backward(begin() + index , end(), begin() + index + 1);

    m_values[index] = elem;
}

void DoubleVec::shrink()
{
    // Check if we can shrink the vector without losing elements.
    const std::size_t new_size = m_count >= m_size * SHRINK_MAX_DECREASE
        ? m_count                       // Shrink to m_count == m_size.
        : m_size * SHRINK_MAX_DECREASE; // Shrink m_size by the max decrease.

    auto* const new_values = new DoubleVec::Elem[new_size];

    std::copy(begin(), end(), new_values);

    delete[] m_values;
    m_values = new_values;
    m_size = new_size;
}


