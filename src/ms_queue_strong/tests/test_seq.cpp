////
//// Created by alg on 11/25/23.
////

#include <iostream>
#include <cassert>
#include "../ms_queue_strong.h"

/*
 * Sequential testing
 */

// Test queue initialization
void test_init() {
    MSQueueStrong<int> queue;
    Pointer<int> head = queue.head;
    Pointer<int> tail = queue.tail;

    // Check the head
    assert(head.count == 0);

    // Check the tail
    assert(tail.count == 0);

    // Check that head and tail point to the same fake node
    assert(head.ptr == tail.ptr);
}

//  Test enqueue
void test_enqueue() {
    MSQueueStrong<int> queue;
    queue.enqueue(5);
    queue.enqueue(7);
    queue.enqueue(8);
}

// Test enqueue then dequeue
void test_FIFO_1() {
    cout << "### Begin Test: test_FIFO_1 ###" << endl;
    MSQueueStrong<int> queue;
    queue.enqueue(5);
    queue.enqueue(7);
    queue.enqueue(8);
    int val = -1;
    queue.dequeue(&val);
    cout << "Dequeued " << val << endl;
    queue.dequeue(&val);
    cout << "Dequeued " << val << endl;
    queue.dequeue(&val);
    cout << "Dequeued " << val << endl;
    cout << "### End Test: test_FIFO_1 ###" << endl;
}

// Test enqueue then dequeue
void test_FIFO_2() {
    cout << "### Begin Test: test_FIFO_2 ###" << endl;
    MSQueueStrong<int> queue;
    int val = -1;
    queue.enqueue(5);
    queue.dequeue(&val);
    cout << "Dequeued " << val << endl;
    queue.enqueue(7);
    queue.dequeue(&val);
    cout << "Dequeued " << val << endl;
    queue.enqueue(8);
    queue.dequeue(&val);
    cout << "Dequeued " << val << endl;
    cout << "### End Test: test_FIFO_2 ###" << endl;
}

// Test enqueue then dequeue 1 more time than dequeue
void test_FIFO_3() {
    cout << "### Begin Test: test_FIFO_3 ###" << endl;
    MSQueueStrong<int> queue;
    int val = -1;
    queue.enqueue(5);
    queue.dequeue(&val);
    cout << "Dequeued " << val << endl;
    queue.enqueue(7);
    queue.dequeue(&val);
    cout << "Dequeued " << val << endl;
    queue.enqueue(8);
    queue.dequeue(&val);
    cout << "Dequeued " << val << endl;
    assert(queue.dequeue(&val) == false);
    cout << "### End Test: test_FIFO_3 ###" << endl;
}

// Enqueue and then dequeue 1M items
void test_FIFO_1M() {
    cout << "### Begin Test: test_FIFO_1M ###" << endl;
    MSQueueStrong<int> queue;
    int val = -1;
    cout << "Start enqueuing..." << endl;
    for (int i = 0; i < 1e6; i++) {
        queue.enqueue(i);
    }
    cout << "Finished enqueuing" << endl;
    cout << "Start dequeuing" << endl;
    for (int i = 0; i < 1e6; i++) {
        queue.dequeue(&val);
        assert(val == i);
//        cout << "Dequeued " << val << endl;
    }
    cout << "Finished dequeuing" << endl;
    cout << "### End Test: test_FIFO_1M ###" << endl;
}


int main() {
    // Run some sequential tests
    test_init();
    test_enqueue();
    test_FIFO_1();
    test_FIFO_2();
    test_FIFO_3();
    test_FIFO_1M();

    cout << "Finished testing" << endl;
    return 0;
}
