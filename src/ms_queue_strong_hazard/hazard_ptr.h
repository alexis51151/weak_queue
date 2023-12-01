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

//template <typename T>
//int binarySearch(const std::vector<T>& arr, T target) {
//    int left = 0;
//    int right = arr.size() - 1;
//
//    while (left <= right) {
//        int mid = left + (right - left) / 2;
//
//        // Check if the target is present at the middle
//        if (arr[mid] == target) {
//            return mid;
//        }
//
//        // If the target is greater, ignore the left half
//        if (arr[mid] < target) {
//            left = mid + 1;
//        }
//            // If the target is smaller, ignore the right half
//        else {
//            right = mid - 1;
//        }
//    }
//
//    // If the target is not present in the array
//    return -1;
//}

// Simplified Hazard Pointer implementation
// when you need K hazard ptr record.
// Number of threads is fixed.

// N: number of threads
// K: number of hazard pointers
template<class T, int N, int K>
class HazardRec {
public:
    // Each thread has its own version of the hazard pointer
    atomic<T*> hp[N][K] = {nullptr};
    int threshold = 5;

    void retire_node(T* node, vector<T*>& free_list) {
        // retire the node (to be freed)
        free_list.push_back(node);
        if (free_list.size() >= threshold) {
            this->scan();
        }
    }

    // tid = thread id of the thread calling scan
    void scan(int tid, vector<T*>& free_list) {
        vector<T*> ptr_list;
        // Look at the other threads versions of the hazard pointers
        for (int i = 0; i < N; i++) {
            if (i != tid) {
                for (atomic<T*> hptr : hp[i]) {
                    T* ptr = hptr.load();
                    if (ptr != nullptr) {
                        ptr_list.push_back(ptr);
                    }
                }
            }
        }

        // Search if you can free any node from free_list
        vector<T*> new_free_list;
        // Sort the free_list
        sort(free_list);
        while (!free_list.empty()) {
            T* node = free_list.pop_back();
            // Another thread still has a copy
            if (binary_search(free_list.begin(), free_list.end(), node)) {
                new_free_list.push_back(node);
            }
            // No other thread has a copy
            else {
                delete node;
            }
        }
    }
};
#endif //COMP_522_HAZARD_PTR_H
