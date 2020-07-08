//
// Created by brian on 7/8/20.
//

#include <iostream>
#include "double_vec.h"


int main() {
    DoubleVec vec{10};
    vec.append(7);
    vec.shrink();

    for (auto v : vec) {
        std::cout << v << ' ';
    }

}
