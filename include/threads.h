#include "c_ucontext.h"

typedef int thread_id;

typedef enum thread_ct_state_ {READY, RUNNING, WAITING, TERMINATED} thread_ct_state;

typedef struct thread_ct_ {
	thread_id 		id;
	routine 		worker;
	ucontext_ct 	*context;
	thread_ct_state state;
} thread_ct;
