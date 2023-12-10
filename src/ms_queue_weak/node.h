//
// Created by alg on 11/24/23.
//

#ifndef COMP_522_NODE_H
#define COMP_522_NODE_H

#include <atomic>
#include "pointer.h"

using namespace std;

template<class T>
class Node {
public:
    T value;
    atomic<Pointer<T>> next;

    Node() {
        next.store(Pointer<T>(nullptr, 0), memory_order_relaxed);
    }

    // Create node with no successor
    explicit Node(const T& val) {
        value = val;
        next.store(Pointer<T>(nullptr, 0), memory_order_relaxed);
    }
};

#endif //COMP_522_NODE_H
