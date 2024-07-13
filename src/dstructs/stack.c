#include "../../include/dstructs/stack.h"

int stack_push(stack_ds *s, const void *data)
{
	return llist_ins_next(s, NULL, data);
}

int stack_pop(stack_ds *s, void **data) 
{
	return llist_rem_next(s, NULL, data);
}