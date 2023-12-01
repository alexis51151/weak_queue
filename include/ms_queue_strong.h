//
// Created by alg on 11/23/23.
//

/*
 * Michael and Scott Non-Blocking queue implementation for a strong memory model.
 */

#ifndef COMP_522_MS_QUEUE_STRONG_H
#define COMP_522_MS_QUEUE_STRONG_H

#include "pointer.h"
#include <atomic>

template<class T>
class MSQueue {
public:
    // Head and Tail must be accessed atomically
    atomic<Pointer<T>> head;
    atomic<Pointer<T>> tail;

    MSQueue();

    void enqueue(const T &value);

    bool dequeue(T *value);
};
#endif //COMP_522_MS_QUEUE_STRONG_H