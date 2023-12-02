include ../benchmarks.mk

TESTNAME = ms_queue_strong

all: $(TESTNAME)

$(TESTNAME): $(TESTNAME).cpp $(TESTNAME).h node.h
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TESTNAME) *.o
