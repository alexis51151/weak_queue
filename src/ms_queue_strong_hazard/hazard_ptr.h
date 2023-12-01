//
// Created by alg on 11/30/23.
//

#ifndef COMP_522_HAZARD_PTR_H
#define COMP_522_HAZARD_PTR_H

/*
 * Hazard Pointer (2004) to tackle the ABA problem without usind DWORD CAS
 */

#include <atomic>
#include <vector>
#include <algorithm>

using namespace std;

// Simplified Hazard Pointer implementation
// when you need K hazard ptr record.
// Number of threads is fixed.

// N: number of threads
// K: number of hazard pointers
template<class T, int N, int K>
class HazardRec {
public:
    // Each thread has its own version of the hazard pointer
    atomic<T*> hp[N][K];
    int threshold;

    HazardRec() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < K; j++) {
                hp[i][j].store(nullptr);
            }
        }
        threshold = 5;
    }

    void retire_node(T* node, int tid, vector<T*>& free_list) {
        // retire the node (to be freed)
        free_list.push_back(node);
        if (free_list.size() >= threshold) {
            this->scan(tid, free_list);
        }
    }

    // tid = thread id of the thread calling scan
    void scan(int tid, vector<T*>& free_list) {
        vector<T*> ptr_list;
        // Look at the other threads versions of the hazard pointers
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < K; j++) {
                T* ptr = hp[i][j].load();
                if (ptr != nullptr) {
                    ptr_list.push_back(ptr);
                }
            }
        }

        // Search if you can free any node from free_list
        vector<T*> new_free_list;
        // Sort the ptr_list (for binary search)
        sort(ptr_list.begin(), ptr_list.end(), [](T* a, T*b) { return a < b; });
        while (!free_list.empty()) {
            T* node = free_list.back();
            free_list.pop_back();
            // Another thread still has a copy
            if (binary_search(ptr_list.begin(), ptr_list.end(), node, [](T* a, T*b) { return a < b; })) {
                new_free_list.push_back(node);
            }
            // No other thread has a copy
            else {
                cout << "Freed node " << node->value << endl;
                delete node;
            }
        }

        // Update the free list
        free_list = new_free_list;
    }
};
#endif //COMP_522_HAZARD_PTR_H
