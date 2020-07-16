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

    LinkedList<int> z{};
//
    auto it = z.before_begin();
//
    z.insert(it, 7);

    for(auto elem : z) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';
}
