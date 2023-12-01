//
// Created by alg on 11/23/23.
//

/*
 * Michael and Scott Non-Blocking queue implementation for a strong memory model.
 */

#ifndef COMP_522_MS_QUEUE_STRONG_H
#define COMP_522_MS_QUEUE_STRONG_H

#include "node.h"
#include "atomic"
#include "hazard_ptr.h"


template<class T, int N>
class MSQueue {
public:
    // Head and Tail must be accessed atomically
    atomic<Node<T>*> head;
    atomic<Node<T>*> tail;
    // Hazard pointers (need 1 for enqueue and 2 for dequeue)
    HazardRec<Node<T>, N, 2> hazard;

    MSQueue() {
        // Create the initial node: fake node
        Node<T>* fake = new Node<T>();
        // This node has no successor
        this->head.store(fake);
        this->tail.store(fake);
    }

    void enqueue(const T &value, int tid) {
        // Create a new node
        Node<T>* node = new Node<T>(value);
        Node<T>* cur_tail;
        while (true) {
            // Retrieve the current tail and last inserted node
            cur_tail = this->tail.load();
            // Update the hazard pointer (access hazard with cur_tail)
            hazard.hp[tid][0].store(cur_tail);
            // Check that the tail is safe
            if (this->tail.load() != cur_tail)
                continue;
            // Read the next element
            Node<T>* next = cur_tail->next.load();
            // If the tail has changed, try again (same idea as Test-And-Test-And-Set)
            if (this->tail.load() == cur_tail) {
                // Is the tail lagging behind?
                if (next == nullptr) {
                    // If not, try to insert the node
                    if (cur_tail->next.compare_exchange_strong(next, node)) {
                        // New node successfully added
                        break;
                    }
                } else {
                    // Tail is lagging behind
                    // Try to update the tail, and try again to insert later
                    // If it fails, someone else has updated the tail (progress condition)
                    this->tail.compare_exchange_strong(cur_tail, next);
                }
            }
        }
        // Try to update the tail
        // If it fails, someone else has updated the tail (progress condition)
        this->tail.compare_exchange_strong(cur_tail, node);
    }

    bool dequeue(T* value, int tid, vector<Node<T>*>& free_list) {
        Node<T>* cur_head;
        while (true) {
            cur_head = this->head.load();
            // Update the hazard pointer (access hazard with cur_head)
            hazard.hp[tid][0].store(cur_head);
            // Check that the head is safe
            if (this->head.load() != cur_head)
                continue;
            Node<T>* cur_tail = this->tail.load();
            Node<T>* next = cur_head->next.load();
            // Update the hazard pointer
            hazard.hp[tid][1].store(next);
            // Check if head, tail and next are consistent
            if (this->head.load() == cur_head) {
                // Is queue empty or tail lagging behind?
                if (cur_head == cur_tail) {
                    // Is queue empty?
                    if (next == nullptr) {
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
        // Free the node (only the thread that succeeded CAS can free it)
        // Use of hazard pointer to safely free the node
        hazard.retire_node(cur_head, tid, free_list);
//        delete cur_head;
        // Queue was not empty, so the dequeue is successful
        return true;
    }

    // Sequential interface

    void enqueue(const T &value) {
        enqueue(value, 0);
    }

    bool dequeue(T* value) {
        vector<Node<T>*> free_list;
        return dequeue(value, 0, free_list);
    }
};
#endif //COMP_522_MS_QUEUE_STRONG_H