//
// Created by alg on 11/24/23.
//

#ifndef COMP_522_NODE_H
#define COMP_522_NODE_H

#include <atomic>

using namespace std;

template<class T>
class Node {
public:
    T value;
    atomic<Node<T>*> next;

    Node() {
        next.store(nullptr);
    }

    // Create node with no successor
    explicit Node(const T& val) {
        value = val;
    }
};

#endif //COMP_522_NODE_H
