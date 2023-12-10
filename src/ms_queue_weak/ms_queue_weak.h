//
// Created by alg on 11/23/23.
//

/*
 * Michael and Scott Non-Blocking queue implementation for a strong memory model.
 */

#ifndef COMP_522_MS_QUEUE_WEAK_H
#define COMP_522_MS_QUEUE_WEAK_H

#include "node.h"
#include "pointer.h"
#include "atomic"

template<class T>
class MSQueue {
public:
    // Head and Tail must be accessed atomically
    atomic<Pointer<T>> head;
    atomic<Pointer<T>> tail;

    MSQueue() {
        // Create the initial node: fake node
        Node<T>* fake = new Node<T>();
        // This node has no successor
        this->head.store(Pointer<T>(fake, 0), memory_order_relaxed);
        this->tail.store(Pointer<T>(fake, 0), memory_order_relaxed);
    }

    void enqueue(const T &value) {
        // Create a new node
        Node<T>* node = new Node<T>(value);
        Pointer<T> cur_tail;
        while (true) {
            // Retrieve the current tail and last inserted node
            cur_tail = this->tail.load(memory_order_acquire);
            Pointer<T> next = cur_tail.ptr->next.load(memory_order_acquire);
            // If the tail has changed, try again (same idea as Test-And-Test-And-Set)
            if (this->tail.load(memory_order_relaxed) == cur_tail) {
                // Is the tail lagging behind?
                if (next.ptr == nullptr) {
                    // Try to insert the node
                    if (cur_tail.ptr->next.compare_exchange_strong(next, Pointer<T>(node, next.count + 1),
                            memory_order_release, memory_order_relaxed)) {
                        // New node successfully added
                        break;
                    }
                } else {
                    // Try to update the tail, and try again to insert later
                    // If it fails, someone else has updated the tail
                    this->tail.compare_exchange_weak(cur_tail, Pointer<T>(next.ptr, cur_tail.count + 1),
                            memory_order_relaxed, memory_order_relaxed);
                }
            }
        }
        // Try to update the tail
        // If it fails, someone else has updated the tail
        this->tail.compare_exchange_weak(cur_tail, Pointer<T>(node, cur_tail.count + 1),
                memory_order_release, memory_order_relaxed);
    }

    bool dequeue(T *value) {
        Pointer<T> cur_head;
        while (true) {
            cur_head = this->head.load(memory_order_acquire);
            Pointer<T> cur_tail = this->tail.load(memory_order_relaxed);
            Pointer<T> next = cur_head.ptr->next.load(memory_order_acquire);
            // Check if head, tail and next are consistent
            if (this->head.load(memory_order_relaxed) == cur_head) {
                // Is queue empty or tail lagging behind?
                if (cur_head.ptr == cur_tail.ptr) {
                    // Is queue empty?
                    if (next.ptr == nullptr) {
                        return false;
                    }
                    // Tail is lagging behind
                    this->tail.compare_exchange_weak(cur_tail, Pointer<T>(next.ptr, cur_tail.count + 1),
                            memory_order_relaxed, memory_order_relaxed);
                } else {
                    // Queue is not empty
                    // Read value before CAS to prevent another dequeue from freeing the same node
                    *value = next.ptr->value;
                    // Try to update head to next node
                    if (this->head.compare_exchange_strong(cur_head, Pointer<T>(next.ptr, cur_head.count + 1),
                            memory_order_release, memory_order_relaxed)) {
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
#endif //COMP_522_MS_QUEUE_WEAK_H