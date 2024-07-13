#include "../include/threads.h"
#include "../include/dstructs/bitmap.h"

static bitmap thread_ids_bitmap;
static cthread cthreads[MAX_NUM_OF_THREADS];

cthread_id allocator_allocate_id(void)
{
	if (thread_ids_bitmap.bits == NULL) return -1;
	cthread_id id = bitmap_find_first(&thread_ids_bitmap, false);
	bitmap_set_bit(&thread_ids_bitmap, id, true);
	
	return id;
}

cthread_id cthread_create(routine worker, 
						  void *args, 
						  intmax_t stack_size)
{
	if (thread_ids_bitmap.bits == NULL && 
		bitmap_create(&thread_ids_bitmap, MAX_NUM_OF_THREADS) == -1) return -1;
	if (stack_size < -1) return -1;
	if (stack_size == -1) stack_size = THREAD_DEFAULT_STACK_SIZE;

	cthread_id id = allocator_allocate_id();
	if (id == -1) return id;

	ucontext_ct *context;
	if (!(context = malloc(sizeof(ucontext_ct)))) return -1;
	void *bp;
	if (!(bp = malloc(stack_size))) return -1;
	void *sp = bp;
	context->stack.bp = bp; context->stack.sp = sp; context->stack.size = stack_size;
	getcontext_ct(context);
	makecontext_ct(context, worker, args);
	cthreads[id].context = context;
	cthreads[id].id = id;
	cthreads[id].state = READY;

	return id;
}