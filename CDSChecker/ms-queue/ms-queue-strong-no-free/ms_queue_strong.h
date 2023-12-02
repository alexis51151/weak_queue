//
// Created by alg on 11/23/23.
//

/*
 * Michael and Scott Non-Blocking queue implementation for a strong memory model.
 */

#ifndef COMP_522_MS_QUEUE_STRONG_H
#define COMP_522_MS_QUEUE_STRONG_H

#include "node.h"
#include <stdatomic.h>
#include <unrelacy.h>

class MSQueue {
public:
    // Head and Tail must be accessed atomically
    atomic<Node*> head;
    atomic<Node*> tail;

    MSQueue() {
        // Create the initial node: fake node
        Node* fake = new Node();
        // This node has no successor
        this->head.store(fake);
        this->tail.store(fake);
    }

    void enqueue(uint32_t value) {
        // Create a new node
        Node* node = new Node(value);
        Node* cur_tail;
        while (true) {
            // Retrieve the current tail and last inserted node
            cur_tail = this->tail.load();
            Node* next = cur_tail->next.load();
            // If the tail has changed, try again (same idea as Test-And-Test-And-Set)
            if (this->tail.load() == cur_tail) {
                // Is the tail lagging behind?
                if (next == NULL) {
                    // Try to insert the node
                    if (cur_tail->next.compare_exchange_strong(next, node)) {
                        // New node successfully added
                        break;
                    }
                } else {
                    // Try to update the tail, and try again to insert later
                    // If it fails, someone else has updated the tail
                    this->tail.compare_exchange_strong(cur_tail, next);
                }
            }
        }
        // Try to update the tail
        // If it fails, someone else has updated the tail
        this->tail.compare_exchange_strong(cur_tail, node);
    }

    bool dequeue(uint32_t* value) {
        Node* cur_head;
        while (true) {
            cur_head = this->head.load();
            Node* cur_tail = this->tail.load();
            Node* next = cur_head->next.load();
            // Check if head, tail and next are consistent
            if (this->head.load() == cur_head) {
                // Is queue empty or tail lagging behind?
                if (cur_head == cur_tail) {
                    // Is queue empty?
                    if (next == NULL) {
                        return false;
                    }
                    // Tail is lagging behind
                    this->tail.compare_exchange_strong(cur_tail, next);
                } else {
                    // Queue is not empty
                    // Read value before CAS to prevent another dequeue from freeing the same node
                    *value = next->value;
                    // Try to update head to next node
                    if (this->head.compare_exchange_strong(cur_head, next)) {
                        // Dequeue successful
                        break;
                    }
                }
            }
        }
        // Queue was not empty, so the dequeue is successful
        return true;
    }
};
#endif //COMP_522_MS_QUEUE_STRONG_H