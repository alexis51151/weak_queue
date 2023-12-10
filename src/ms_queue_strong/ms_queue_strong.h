//
// Created by alg on 11/23/23.
//

/*
 * Michael and Scott Non-Blocking queue implementation for a strong memory model.
 */

#ifndef COMP_522_MS_QUEUE_STRONG_H
#define COMP_522_MS_QUEUE_STRONG_H

#include "node.h"
#include "pointer.h"
#include <atomic>

template<class T>
class MSQueueStrong {
public:
    // Head and Tail must be accessed atomically
    atomic<Pointer<T>> head;
    atomic<Pointer<T>> tail;

    explicit MSQueueStrong() {
        // Create the initial node: fake node
        Node<T>* fake = new Node<T>(0);
        // This node has no successor
        atomic_init(&head, Pointer<T>(fake, 0));
        atomic_init(&tail, Pointer<T>(fake, 0));
//        this->head.store(Pointer<T>(fake, 0));
//        this->tail.store(Pointer<T>(fake, 0));
    }

    void enqueue(const T &value) {
        // Create a new node
        Node<T>* node = new Node<T>(value);
        Pointer<T> cur_tail(nullptr, 0);
        while (true) {
            // Retrieve the current tail and last inserted node
            cur_tail = this->tail.load();
            Pointer<T> next = cur_tail.ptr->next.load();
            // If the tail has changed, try again (same idea as Test-And-Test-And-Set)
            if (this->tail.load() == cur_tail) {
                // Is the tail lagging behind?
                if (next.ptr == nullptr) {
                    // Try to insert the node
                    if (cur_tail.ptr->next.compare_exchange_strong(next, Pointer<T>(node, next.count + 1))) {
                        // New node successfully added
                        break;
                    }
                } else {
                    // Try to update the tail, and try again to insert later
                    // If it fails, someone else has updated the tail
                    this->tail.compare_exchange_strong(cur_tail, Pointer<T>(next.ptr, cur_tail.count + 1));
                }
            }
        }
        // Try to update the tail
        // If it fails, someone else has updated the tail
        this->tail.compare_exchange_strong(cur_tail, Pointer<T>(node, cur_tail.count + 1));
    }

    bool dequeue(T *value) {
        Pointer<T> cur_head;
        while (true) {
            cur_head = this->head.load();
            Pointer<T> cur_tail = this->tail.load();
            Pointer<T> next = cur_head.ptr->next.load();
            // Check if head, tail and next are consistent
            if (this->head.load() == cur_head) {
                // Is queue empty or tail lagging behind?
                if (cur_head.ptr == cur_tail.ptr) {
                    // Is queue empty?
                    if (next.ptr == nullptr) {
                        return false;
                    }
                    // Tail is lagging behind
                    this->tail.compare_exchange_strong(cur_tail, Pointer<T>(next.ptr, cur_tail.count + 1));
                } else {
                    // Queue is not empty
                    // Read value before CAS to prevent another dequeue from freeing the same node
                    *value = next.ptr->value;
                    // Try to update head to next node
                    if (this->head.compare_exchange_strong(cur_head, Pointer<T>(next.ptr, cur_head.count + 1))) {
                        // Dequeue successful
                        break;
                    }
                }
            }
        }
        // Free the node (only the thread that succeeded CAS can free it)
        delete cur_head.ptr;
        // Queue was not empty, so the dequeue is successful
        return true;
    }
};
#endif //COMP_522_MS_QUEUE_STRONG_H