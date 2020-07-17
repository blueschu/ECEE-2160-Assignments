/*
 * ECEE 2160 Lab Assignment 2 linked list definitions.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-16
 *
 */

template<typename T>
typename LinkedList<T>::iterator  // typename keyword needed for dependent return type
LinkedList<T>::insert_after(LinkedList<T>::iterator position, const T& value)
{
    // Create a new node which takes ownership of the current "next node".
    auto new_node = std::unique_ptr<BaseNode>(new Node{
        std::move(position.m_iter_pos->m_next_ptr), // Transfer node ownership
        value   // Copy value into new heap allocated node
    });

    // Set new_node as the current node's "next node".
    // The current node takes ownership of new_node.
    std::swap(position.m_iter_pos->m_next_ptr, new_node);

    // The local new_node no longer owns any memory.

    return position.next();
}

template<typename T>
void LinkedList<T>::push_front(const T& value)
{
    insert_after(before_begin(), value);
}

template<typename T>
void LinkedList<T>::remove_after(LinkedList::iterator position)
{
    // Reference to the pointer held by the current node for convenience.
    std::unique_ptr<BaseNode>& next_node_ptr = position.m_iter_pos->m_next_ptr;

    std::unique_ptr<BaseNode> tmp{nullptr};

    // Given ownership of the node after "next node" to the temporary variable
    // on the stack.
    // This is the node that we want the current node to own.
    std::swap(tmp, next_node_ptr->m_next_ptr);
    // Swap the node after "next node" into the current node, and give the
    // pointer to the "next node" to the temporary.
    std::swap(tmp, next_node_ptr);

    // The former "next node" will be desctructed when tmp goes out of scope.
}
