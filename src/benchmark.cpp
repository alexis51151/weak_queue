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

#include "ms_queue_strong/ms_queue_strong.h"
#include "ms_queue_weak/ms_queue_weak.h"


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
void enqueue_dequeue(Q& queue) {
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
        thread t = thread(enqueue_dequeue<Q, M / NTHREADS, N / NTHREADS, N_ITER>, ref(queue));
        threads.push_back(move(t));
    }

    // Join the threads
    for (thread& t : threads) {
        t.join();
    }

    // Stop measuring
    auto end = chrono::high_resolution_clock::now();

    // Compute duration
    double dur_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return dur_ms;
}

template<int N_ITER>
void measure_work() {
    auto start= chrono::high_resolution_clock::now();
    work<N_ITER>();
    auto end = chrono::high_resolution_clock::now();
    double dur_work_ms = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    cout << "work duration = " << dur_work_ms << " ns" << endl;
}


#define N_THREADS 24
int main() {
    cout << "Benchmarking started" << endl;

    // Print the number of threads
    cout << "N_THREADS = " << N_THREADS << endl;

    // Duration of the work
    measure_work<100>();

    // Strong queue
    double dur_strong_ms = bench<MSQueueStrong<int>, 10000000, 10000000, 100, N_THREADS> ();
    cout << "Duration for strong queue = " << dur_strong_ms << "ms" << endl;

    // Weak queue
    double dur_weak_ms = bench<MSQueue<int>, 10000000, 10000000, 100, N_THREADS>();
    cout << "Duration for weak queue = " << dur_weak_ms << " ms" << endl;

    cout << "Benchmarking ended" << endl;
}