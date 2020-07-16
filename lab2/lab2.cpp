/*
 * ECEE 2160 Lab Assignment 2 main file
 *
 * Author:  Brian Schubert
 * Date:    2020-07-16
 *
 */

#include "linked_list.h"

#include <iostream>

int main() {

    LinkedList<int> z;
//
    auto it = z.before_begin();
//
    z.insert_after(it, 7);
    z.push_front(8);
    z.push_front(9);

    for(const auto elem : z) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';

    z.remove_after(z.begin());
    for(const auto elem : z) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';
}
