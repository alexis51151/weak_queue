//
// Created by alg on 11/27/23.
//

#include <iostream>
#include <cassert>
#include "../ms_queue_strong.h"
#include <thread>
#include <vector>
#include <algorithm>

/*
 * Parallel testing
 */

thread_local vector<Node<int>*> free_list;

// sequential enqueue
template<int NTHREADS>
vector<int> enqueue_dequeue_n(MSQueue<int, NTHREADS>& queue, int tid, int n, vector<int>& output) {
    // Enqueuing
    int n_elements = n / NTHREADS;
    int offset = n_elements * tid;
    for (int i = 0; i < n_elements; i++) {
        queue.enqueue(offset + i, tid);
        cout << "[" << tid << "]: enqueued " << offset + i << endl;
    }
    // Dequeuing
    for (int i = 0; i < n_elements; i++) {
        int val = -1;
        // Blocking dequeue
        while (val == -1) {
            cout << "[" << tid << "]: enqueued " << i << endl;
            queue.dequeue(&val, tid, free_list);
        }
        cout << "[" << tid << "]: dequeued " << i << endl;
        assert(val < n);
        output.push_back(val);
    }
    return output;
}


// Enqueue and then dequeue 1M items
template<int NTHREADS>
void test_FIFO_par(int n) {
    cout << "### Begin Test: test_FIFO_1M ###" << endl;
    MSQueue<int, NTHREADS> queue;
    // Invoke the threads
    vector<thread> threads;
    vector<vector<int>> res(NTHREADS, vector<int>());
    for (int i = 0; i < NTHREADS; i++) {
        thread t = thread(enqueue_dequeue_n<NTHREADS>, ref(queue), i, n, ref(res[i]));
        threads.push_back(move(t));
    }
    // Join the threads
    cout << "nthreads = " << NTHREADS << endl;
    for (thread& t : threads) {
        t.join();
    }
    // Merge the vectors
    vector<int> output;
    for (vector<int>& v : res) {
        output.insert(output.end(), v.begin(), v.end());
    }
    // Sort output
    sort(output.begin(), output.end());
    // Verify that all the values were dequeued successfully
    assert(output.size() == n);
    cout << "TEST" << endl;
    for (int i = 0; i < n; i++) {
        assert(output[i] == i);
    }
    cout << "### End Test: test_FIFO_1M ###" << endl;
}


int main() {
    // Run some parallel tests
    test_FIFO_par<2>(1e5);
    cout << "Finished parallel testing" << endl;
    return 0;
}
