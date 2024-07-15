#include "../include/threads.h"
#include "stdio.h"

void foo(void)
{
	puts("hello from foo");
	return;
}

int main(void)
{
	cthread_id id = cthread_create(foo, NULL, -1);

	return EXIT_SUCCESS;
}