//
// Created by alg on 11/24/23.
//

#ifndef COMP_522_POINTER_H
#define COMP_522_POINTER_H

#include <cstdint>

using namespace std;

template<class T>
class Node;


template<class T>
class Pointer {
public:
    Node<T>* ptr;
    uint32_t count; // counter for the ABA problem

    // Default constructor: only called for the fake node
//    explicit Pointer() {
//        ptr = nullptr;
//        count = 0; // initialize counter
//    }

    Pointer(Node<T>* new_ptr, uint64_t new_count) {
        ptr = new_ptr;
        count = new_count;
    }

    bool operator==(const Pointer<T>& other) {
        return this->ptr == other.ptr && this->count == other.count;
    }

};


#endif //COMP_522_POINTER_H
