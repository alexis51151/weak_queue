#include <stdlib.h>
#include <stdio.h>
#include <threads.h>

#include "ms_queue_strong.h"
#include "model-assert.h"

//static MSQueue *queue;
//static unsigned int *input;
//static unsigned int *output;


struct args {
    uint32_t tid;
    MSQueue* queue;
    uint32_t* input;
    uint32_t* output;
    bool* succ1;
    bool* succ2;
};

static void main_task(void* args) {
    struct args* params = (struct args*) args;
    if (!params->tid) {
		params->input[0] = 17;
		params->queue->enqueue(params->input[0]);
		*(params->succ1) = params->queue->dequeue(&params->output[0]);
	} else {
		params->input[1] = 37;
		params->queue->enqueue(params->input[1]);
		*(params->succ2) = params->queue->dequeue(&params->output[1]);
	}
}

#define NTHREADS 2
int user_main(int argc, char **argv) {
	uint32_t in_sum = 0;
    uint32_t out_sum = 0;
    bool succ1, succ2;

	MSQueue queue;

    thrd_t threads[NTHREADS];
    struct args param[NTHREADS];
    uint32_t input[NTHREADS];
    uint32_t output[NTHREADS];

	for (uint32_t i = 0; i < NTHREADS; i++) {
		param[i] = {i, &queue, input, output, &succ1, &succ2};
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
	if (succ1 && succ2)
		MODEL_ASSERT(in_sum == out_sum);
	else
		MODEL_ASSERT (false);

	return 0;
}
