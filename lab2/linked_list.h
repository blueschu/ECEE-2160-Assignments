/*
 * ECEE 2160 Lab Assignment 2 linked list declaration.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-16
 *
 * References
 * ===========
 *
 *  [1] https://rust-unofficial.github.io/too-many-lists/index.html
 *  [2] https://en.cppreference.com/w/cpp/memory/unique_ptr
 *  [3] https://en.cppreference.com/w/cpp/container/forward_list
 *  [4] https://en.cppreference.com/w/cpp/named_req/ForwardIterator
 *  [5] https://stackoverflow.com/a/38103394
 *
 */

#ifndef ECEE_2160_LAB_REPORTS_LINKED_LIST_H
#define ECEE_2160_LAB_REPORTS_LINKED_LIST_H

#include <memory>           // for std::unique_ptr

/**
 * A singlely linked list.
 *
 * This implementation attempts to implement a similar interface to that of
 * `std::forward_list` from the C++ standard library.
 * @tparam T The data type of elements stored in the list.
 */
template<typename T>
class LinkedList {

    /**
     * Helper class representing a linked list element with no data.
     *
     * This technique is adapted from one used in the C++ standard library
     * sources included with my compiler. By defining separate classes for
     * nodes with data and nodes without data, we gain the ability of having
     * a unified interface for accessing the head and tail of the list.
     */
    struct BaseNode {
        /// Owning pointer to the next node.
        std::unique_ptr<BaseNode> next;
    };

    /// Helper class representing a link in the linked list.
    struct Node : public BaseNode {
        /// The value contained in this node.
        T value;
    };

    /**
     *  Pointer to the first node in this linked list.
     *
     *  The pointer will be nullptr when the list is empty.
     */

    std::unique_ptr<Node> head{nullptr};

  public:
    /**
     * A forward iterator over a linked list.
     *
     * This class attempts to implement a basic forward iterator [4,5].
     */
    struct iterator {
        /**
         * The position of this iterator in the linked list.
         *
         * While declared as a BaseNode, this pointer MUST reference a Node
         * in order for dereferencing this iterator to be defined. This should
         * always be the case except in the events where this iterator is
         * represents a position directly before or after the list.
         */
        BaseNode* pos;

        /*
         * Standard aliases for iterator traits
         */
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        /**
         * Helper function for computing the iterator for the next element in
         * the list.
         *
         * @return Iterator to next element.
         */
        iterator next() const noexcept {
            return pos ? pos->next : nullptr;
        }

        /*
         * Dereferencing operator overload.
         * We assume that this iterator points a valid node.
         */
        reference operator*() noexcept { return static_cast<Node*>(pos)->value; }

        pointer operator->() noexcept { return static_cast<Node*>(pos)->value; }

        bool operator==(iterator other) const noexcept { return pos == other.pos; }

        bool operator!=(iterator other) const noexcept { return !(*this == other); }

        iterator& operator++() noexcept
        {
            // We assume that operator++ will not be called on an end node.
            pos = pos->next;
            return *this;
        }

        iterator operator++(int) noexcept
        {
            auto temp = *this;
            ++(*this);
            return temp;
        }

    };

//    using iterator = LinkedListIterator<T, Node<T>*>;
//    using const_iterator = LinkedListIterator<T, const Node<T>*>;



  public:
    iterator begin()
    {
        return iterator{head.get()};
    }

    iterator end()
    {
        return iterator{nullptr};
    }

//    const_iterator cbegin() const
//    {
//        return const_iterator{head.get()};
//    }
//
//    const_iterator cend() const
//    {
//        return const_iterator{nullptr};
//    }

    iterator insert(iterator position, T value);

};

#include "linked_list.tpp"

#endif //ECEE_2160_LAB_REPORTS_LINKED_LIST_H
