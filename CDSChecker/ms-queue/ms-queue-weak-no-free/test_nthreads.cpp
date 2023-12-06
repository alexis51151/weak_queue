#include <stdio.h>
#include <threads.h>

#include "ms_queue_weak.h"
#include "model-assert.h"

struct args {
    uint32_t tid;
    MSQueue* queue;
    uint32_t* input;
    uint32_t* output;
    bool* succ;
};

static void main_task(void* args) {
    // Retrieve the arguments
    auto params = (struct args*) args;
    uint32_t* input = params->input;
    uint32_t* output = params->output;
    MSQueue* queue = params->queue;
    bool* succ = params->succ;
    uint32_t tid = params->tid;

    // Perform enqueue and dequeue
    input[tid] = tid;
    queue->enqueue(input[tid]);
    succ[tid] = queue->dequeue(&output[tid]);
}

#define NTHREADS 2
int user_main(int argc, char **argv) {
	uint32_t in_sum = 0;
    uint32_t out_sum = 0;

	MSQueue queue;

    thrd_t threads[NTHREADS];
    struct args param[NTHREADS];
    uint32_t input[NTHREADS];
    uint32_t output[NTHREADS];
    bool succ[NTHREADS];

	for (uint32_t i = 0; i < NTHREADS; i++) {
		param[i] = {i, &queue, input, output, &succ[i]};
		thrd_create(&threads[i], main_task, &param[i]);
	}
	for (auto thread : threads)
		thrd_join(thread);

	for (int i = 0; i < NTHREADS; i++) {
		in_sum += input[i];
		out_sum += output[i];
	}
	for (int i = 0; i < NTHREADS; i++)
		printf("input[%d] = %u\n", i, input[i]);
	for (int i = 0; i < NTHREADS; i++)
		printf("output[%d] = %u\n", i, output[i]);

    // if all the threads successfully dequeued, the input and output sums should be equal.
    bool all_succ = true;
    for (auto s : succ) {
        all_succ &= s;
    }

	if (all_succ)
		MODEL_ASSERT(in_sum == out_sum);

	return 0;
}
