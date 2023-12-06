//
// Created by alg on 11/24/23.
//

#ifndef COMP_522_NODE_H
#define COMP_522_NODE_H

#include <stdatomic.h>
#include <impatomic.h>


class Node {
public:
    unsigned int value;
    std::atomic<Node*> next;

    Node() {
        next.store(NULL, memory_order_relaxed);
    }

    // Create node with no successor
    explicit Node(unsigned int val) {
        value = val;
        next.store(NULL, memory_order_relaxed);
    }
};



#endif //COMP_522_NODE_H
