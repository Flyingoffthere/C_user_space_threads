#pragma once
#include "c_ucontext.h"

#define MAX_NUM_OF_THREADS		  20
#define DEFAULT_STACK_SIZE 		  64 * 1024 

typedef int cthread_id;

typedef enum cthread_state_ {
	READY,
	WAIT,
	RUN,
	TERMINATED,
} cthread_state;

typedef struct cthread_ {
	cthread_id 		id;
	cthread_state 	state;
	ucontext_ct 	*context;
} cthread;


cthread_id cthread_create(routine, 
						  void *args, 
						  intmax_t stack_size);

ucontext_ct *run_thread(cthread_id);

void __cthread_yield(void);
#define yield() __cthread_yield