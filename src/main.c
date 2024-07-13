#include "../include/threads.h"

void foo(void)
{
	return;
}

int main(void)
{
	cthread_id ID;
	ID = 2;
	ID = cthread_create(foo, NULL, -1);
	return EXIT_SUCCESS;
}