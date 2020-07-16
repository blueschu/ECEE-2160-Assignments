/*
 * ECEE 2160 Lab Assignment 2 linked list definitions.
 *
 * Author:  Brian Schubert
 * Date:    2020-07-16
 *
 */

//template<typename T>
//typename LinkedList<T>::iterator LinkedList<T>::insert(LinkedList::const_iterator pos, T value)
//{
//    // Create new node that takes ownership of the next node.
//    std::unique_ptr<Node<T>> temp{nullptr};
//    std::swap(temp, pos)
//
//    auto new_node = Node{value, pos.pos->next.release()};
//
//    pos->next = new_node;
//
//
//}


template<typename T>
typename LinkedList<T>::iterator LinkedList<T>::insert(LinkedList::iterator position, T value)
{
    // Create a new node which takes ownership of the current next node.
    auto new_node = std::unique_ptr<BaseNode>(new Node{
        std::move(position.m_iter_pos->m_next_ptr),
        value
    });

    // Give ownership of the new node to the current node.
    std::swap(position.m_iter_pos->m_next_ptr, new_node);

    // new_node no longer owns any memory

    return position.next();
}
