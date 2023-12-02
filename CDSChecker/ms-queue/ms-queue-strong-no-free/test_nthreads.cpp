#include <threads.h>
#include "ms_queue_strong.h"
#include "model-assert.h"

#define NTHREADS 2
#define N 10
#define N_ITEMS 5

struct args {
    uint32_t tid;
    MSQueue* queue;
    uint32_t* input;
    uint32_t* output;
};

static void main_task(void* args) {
    auto params = (struct args*) args;
    uint32_t tid = params->tid;
    MSQueue* queue = params->queue;
    uint32_t* input = params->input;
    uint32_t* output = params->output;

    uint32_t offset = tid * N_ITEMS;

    // Enqueue
    uint32_t in_sum = 0;
    for (int i = 0; i < N_ITEMS; i++) {
        in_sum += offset + i;
        queue->enqueue(offset + i);
    }

    // Dequeue in blocking mode
    uint32_t out_sum = 0;
    for (int i = 0; i < N_ITEMS; i++) {
        bool success = false;
        uint32_t val = 100 * N; // large value so that we can catch a mistake if the uninitialized value is pushed
        // Blocking dequeue
        // We know it will eventually return because
        // there are as many enqueued items as dequeued items
        while (!success) {
            success = queue->dequeue(&val);
        }
        out_sum += val;
    }

    // Report the values
    input[tid] = in_sum;
    output[tid] = out_sum;
}

int user_main(int argc, char **argv) {
    uint32_t in_sum = 0;
    uint32_t out_sum = 0;

	MSQueue queue;

    thrd_t threads[NTHREADS];
    struct args param[NTHREADS];
    uint32_t input[NTHREADS];
    uint32_t output[NTHREADS];

	for (uint32_t i = 0; i < NTHREADS; i++) {
		param[i] = {i, &queue, input, output};
		thrd_create(&threads[i], main_task, &param[i]);
	}

	for (auto thread : threads)
		thrd_join(thread);

	for (int i = 0; i < NTHREADS; i++) {
		in_sum += input[i];
		out_sum += output[i];
	}

    MODEL_ASSERT(in_sum == out_sum);

	return 0;
}
