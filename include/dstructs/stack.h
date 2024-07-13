#pragma once

#include "llist.h"

typedef llist stack_ds;

#define stack_init 		llist_init 
#define stack_destroy 	llist_destroy

int stack_push(stack_ds *s, const void *data);
int stack_pop(stack_ds *s, void **data);

#define stack_peek(s) ((s)->head == NULL ? NULL : (s)->head->data)
#define stack_size	llist_size