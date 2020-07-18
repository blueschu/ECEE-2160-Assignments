/*
 * ECEE 2160 Lab Assignment 2 bad unique pointer header library.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-18
 *
 * References
 * ===========
 *
 *  [1] https://en.cppreference.com/w/cpp/utility/exchange
 *  [2] https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 */

#ifndef ECEE_2160_LAB_REPORTS_BAD_UNIQUE_H
#define ECEE_2160_LAB_REPORTS_BAD_UNIQUE_H

#include <utility>      // for std::exchange

/**
 * An smart pointer that uniquely owns its contents.
 *
 * This class is an approximate substitute for std::unique_ptr. For this
 * lab assignment, we were required to perform manual memory allocations
 * using `new` and `delete` to store nodes in a linked list. In order to
 * save some headaches from reasoning about pointer ownership, we decided
 * to implement our own mock smart pointer with roughly the same semantics
 * as those exposed by the STL. Throughout the linked list code, each instance
 * of BadUnique should be able to be replaced by std;:unique_ptr with no
 * obvious change in behavior.
 *
 * This should make our linked list implementation much easier to read
 * since only basic knowledge of move semantics is required rather than a full
 * interpretation of invariants imposed by the class.
 *
 * @tparam T The value type referenced by this pointer.
 */
template<typename T>
class BadUnique {
    /// The owning pointer to the resource managed by this class.
    T* m_ptr{nullptr};

  public:

    /*
     * Default constructor.
     *
     * All members are already given in-class member initializers, so we can
     * just use the compiler generated default constructor [C.45,C.80 in 2].
     */
    BadUnique() = default;

    /*
     * Construct a BadUnique that will the resources references by the given
     * pointer.
     */
    explicit BadUnique(T* owned_ptr) : m_ptr{owned_ptr} {}

    // Disallow copy constructor [C.81 in 2].
    BadUnique(const BadUnique&) noexcept = delete;

    // Move constructor
    BadUnique(BadUnique&& other) noexcept
        : m_ptr{std::exchange(other.m_ptr, nullptr)} {}

    // Destructor.
    ~BadUnique()
    {
        delete m_ptr;
    }

    // Disallow copy assignment [C.81 in 2].
    BadUnique& operator=(const BadUnique&) noexcept = delete;

    // Move assignment
    BadUnique& operator=(BadUnique&& other) noexcept
    {
        // Use std::swap explicitly since don't care about specialization here.
        std::swap(m_ptr, other.m_ptr);
        return *this;
    }

    // Dereference operator overload.
    T& operator*() noexcept { return *m_ptr; }

    // Arrow operator overload.
    T* operator->() noexcept { return m_ptr; }

    /**
     * Returns the owned pointer without releasing ownership.
     *
     * @returns Non-owning pointer to stored resource.
     */
    T* get() const noexcept
    {
        return m_ptr;
    }

};

#endif //ECEE_2160_LAB_REPORTS_BAD_UNIQUE_H
