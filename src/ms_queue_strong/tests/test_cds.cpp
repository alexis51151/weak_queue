//
// Created by alg on 11/29/23.
//


/*
 * Test using CDSChecker for Model Checking
 */
#include <threads.h>
#include "../ms_queue_strong.h"
#include <model-assert.h>

#define NTHREADS 10
#define N 1e6

MSQueue<int> queue;

void task(void *arg) {
    int tid = *((int*) arg);
    // Enqueuing
    int n_elements = N / NTHREADS;
    int offset = n_elements * tid;
    for (int i = 0; i < n_elements; i++) {
        queue.enqueue(offset + i);
    }
    // Dequeuing
    for (int i = 0; i < n_elements; i++) {
        int val = -1;
        // Blocking dequeue
        while (val == -1) {
            queue.dequeue(&val);
        }
        MODEL_ASSERT(val < N);
    }
}

int user_main(int argc, char** argv) {
    thrd_t threads[NTHREADS];
    int* thread_ids[NTHREADS];

    // Create the threads
    for(int tid = 0; tid < NTHREADS; tid++) {
        *thread_ids[tid] = tid;
        thrd_create(&threads[tid], task, &thread_ids[tid]);
    }

    // Join the threads
    for(int tid = 0; tid < NTHREADS; tid++) {
        thrd_join(threads[tid]);
    }


    return 0;
}

int main() {
    return 0;
}