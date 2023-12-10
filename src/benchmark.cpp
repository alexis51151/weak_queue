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
#include <cmath>

using namespace std;

#include "ms_queue_strong/ms_queue_strong.h"
#include "ms_queue_weak/ms_queue_weak.h"

// Perform work for some time
static long work(volatile long n) {
    while (n-- > 0);
    return n;
}


// Each thread performs M enqueue and N dequeue
// Q is the type of the queue
template<class Q>
void enqueue_dequeue(Q& queue, int n, int n_iter) {
    // Enqueuing
    for (int i = 0; i < n; i++) {
        work(n_iter);
        queue.enqueue(i);
    }
    // Dequeuing
    int val = -1;
    for (int i = 0; i < n; i++) {
        queue.dequeue(&val);
        work(n_iter);
    }
}

template<class Q>
double bench(int n, int n_iter, int n_threads) {
    Q queue;
    vector<thread> threads;

    // Dispatch the threads
    auto start = chrono::high_resolution_clock::now(); // Start measuring
    for (int i = 0; i < n_threads; i++) {
        thread t = thread(enqueue_dequeue<Q>, ref(queue), n, n_iter);
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


double measure_work(volatile long n) {
    auto start= chrono::high_resolution_clock::now();
    work(n);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(end - start).count();
}


int main() {
    // Parameters
    int n_threads = 2;
    int n_iter = 100;
    int n = 100000000;
    int n_per_thread = n / n_threads;
    cout << "Benchmarking started" << endl;

    // Print the number of threads
    cout << "N_THREADS = " << n_threads << endl;

    // Duration of the work (average)
    double dur_work_ns = 0;
    for (int i = 0; i < 10; i++) {
        dur_work_ns += measure_work(n_iter);
    }
    dur_work_ns /= 100;
    cout << "work duration = " << dur_work_ns << " ns" << endl;

    // Total duration due to work
    // Each thread calls work (2 * n_per_thread)
    double tot_dur_work_ms = 2 * n_per_thread * dur_work_ns / 1e6;
    cout << "total work duration = " << tot_dur_work_ms << " ms" << endl;

    // Strong queue
    double dur_strong_ms = bench<MSQueueStrong<int>>(n_per_thread, n_iter, n_threads);
    cout << "Duration for strong queue = " << dur_strong_ms << "ms" << endl;

    // Weak queue
    double dur_weak_ms = bench<MSQueue<int>>(n_per_thread, n_iter, n_threads);
    cout << "Duration for weak queue = " << dur_weak_ms << " ms" << endl;

    cout << "Benchmarking ended" << endl;
}