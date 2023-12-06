include ../benchmarks.mk


all: test_2threads test_nthreads test_blocking_dequeue

test_2threads: test_2threads.cpp ms_queue_weak.h node.h
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

test_nthreads: test_nthreads.cpp ms_queue_weak.h node.h
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

test_blocking_dequeue: test_blocking_dequeue.cpp ms_queue_weak.h node.h
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)


clean:
	rm -f test_2threads test_nthread test_blocking_dequeue *.o
