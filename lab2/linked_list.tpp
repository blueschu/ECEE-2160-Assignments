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

    if (position == end()) {
        position = std::unique_ptr<Node>(new Node{
            value,
            nullptr
        });

    } else {
        auto new_node = std::unique_ptr<Node>(new Node{
            value,
            nullptr
        });

        position.pos->next = std::move(new_node);
    }



    return iterator{position.pos->next.get()};
}
