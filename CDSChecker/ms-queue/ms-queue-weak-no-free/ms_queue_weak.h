//
// Created by alg on 11/23/23.
//

/*
 * Michael and Scott Non-Blocking queue implementation for a weak memory model.
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
        this->head.store(fake, memory_order_release);
        this->tail.store(fake, memory_order_release);
    }


    // All CAS should have a release memory ordering: the instructions before them should not be ordered after the CAS
    // or we could lose progress and be in an inconsistent state.
    void enqueue(uint32_t value) {
        // Create a new node
        Node* node = new Node(value);
        Node* cur_tail;
        while (true) {
            // Retrieve the current tail and last inserted node
            // Acquire: cur_tail must not be reordered with next,
            // as they are dependant.
            cur_tail = this->tail.load(memory_order_acquire);
            Node* next = cur_tail->next.load(memory_order_acquire);
            // If the tail has changed, try again (same idea as Test-And-Test-And-Set)
            // Relaxed: this is a test to prevent using CAS for nothing, but it is not required
            if (this->tail.load(memory_order_relaxed) == cur_tail) {
                // Is the tail lagging behind?
                if (next == NULL) {
                    // Try to insert the node
                    if (cur_tail->next.compare_exchange_strong(next, node, memory_order_release, memory_order_relaxed)) {
                        // New node successfully added
                        break;
                    }
                } else {
                    // Try to update the tail, and try again to insert later
                    // If it fails, someone else has updated the tail
                    this->tail.compare_exchange_strong(cur_tail, next, memory_order_release, memory_order_relaxed);
                }
            }
        }
        // Try to update the tail
        // If it fails, someone else has updated the tail
        // Release: last instruction, so we just do not want it to be reordered before another instruction
        this->tail.compare_exchange_strong(cur_tail, node, memory_order_release, memory_order_relaxed);
    }

    bool dequeue(uint32_t* value) {
        Node* cur_head;
        while (true) {
            cur_head = this->head.load(memory_order_acquire);
            Node* cur_tail = this->tail.load(memory_order_relaxed);
            Node* next = cur_head->next.load(memory_order_acquire);
            // Check if head, tail and next are consistent
            if (this->head.load(memory_order_relaxed) == cur_head) {
                // Is queue empty or tail lagging behind?
                if (cur_head == cur_tail) {
                    // Is queue empty?
                    if (next == NULL) {
                        return false;
                    }
                    // Tail is lagging behind
                    this->tail.compare_exchange_strong(cur_tail, next, memory_order_release, memory_order_relaxed);
                } else {
                    // Queue is not empty
                    // Read value before CAS to prevent another dequeue from freeing the same node
                    *value = next->value;
                    // Try to update head to next node
                    if (this->head.compare_exchange_strong(cur_head, next, memory_order_release, memory_order_relaxed)) {
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