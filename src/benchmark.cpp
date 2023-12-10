//
// Created by Alexis LE GLAUNEC on 09/12/2023.
//

/*
 * Benchmarking of the Weak and Strong versions of Michael & Scott Nonblocking Queue
 */

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

// Namespace for Strong queue
namespace strong {
    #include "ms_queue_strong/ms_queue_strong.h"
}

// Namespace for Weak queue
namespace weak {
    #include "ms_queue_weak/ms_queue_weak.h"
}

// Dummy computation
template<int N_ITER>
void work() {
    int res = 0;
    for (int i = 0; i < N_ITER; i++) {
        res += 1;
    }
}

// Each thread performs M enqueue and N dequeue
// Q is the type of the queue
template<class Q, int M, int N, int N_ITER>
void enqueue_dequeue(Q queue) {
    // Enqueuing
    for (int i = 0; i < M; i++) {
        if (N_ITER > 0) {
            // Perform some computation
            work<N_ITER>();
        }
        queue.enqueue(i);
    }
    // Dequeuing
    int val = -1;
    for (int i = 0; i < N; i++) {
        queue.dequeue(&val);
        work<N_ITER>();
    }
}

template<class Q, int M, int N, int N_ITER, int NTHREADS>
double bench() {
    Q queue;
    vector<thread> threads;

    // Dispatch the threads
    auto start = chrono::high_resolution_clock::now(); // Start measuring
    for (int i = 0; i < NTHREADS; i++) {
        thread t = thread(enqueue_dequeue<Q, M, N, N_ITER>, ref(queue));
        threads.push_back(move(t));
    }

    // Join the threads
    for (thread& t : threads) {
        t.join();
    }

    // Stop measuring
    auto end = chrono::high_resolution_clock::now();

    // Compute duration
    double dur_ms = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    return dur_ms;
}

int main() {
    cout << "Benchmarking started" << endl;

    // Strong queue
    double dur_strong_ms = bench<strong::MSQueue<int>, 1000000, 1000000, 1000, 2>();
    cout << "Duration for strong queue = " << dur_strong_ms << " ms" << endl;

//    // Weak queue
//    double dur_weak_ms = bench<weak::MSQueue<int>, 1000000, 1000000, 1000, 2>();
//    cout << "Duration for weak queue = " << dur_weak_ms << " ms" << endl;

    cout << "Benchmarking ended" << endl;
}