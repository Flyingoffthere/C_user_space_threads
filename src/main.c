#include "../include/custom_ucontext.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

// tests

ucontext_ct foo_ctx, goo_ctx;

void goo(void)
{
	puts("GOO: doing work");
	return;	
}

void foo(void)
{
	puts("FOO: doing work");
	goo_ctx.uc_link = &foo_ctx;
	makecontext_ct(&goo_ctx, &goo);
	swapcontext_ct(&foo_ctx, &goo_ctx);
	puts("FOO: Goo returned");
	return;
}


int main(void)
{

	char *foo_stack = malloc(sizeof(char) * 1024 * 64);
	getcontext_ct(&foo_ctx);
	foo_ctx.stack.bp = foo_stack;
	foo_ctx.stack.sp = foo_stack;
	foo_ctx.stack.size = sizeof(char) * 1024 * 64;
	makecontext_ct(&foo_ctx, &foo);

	char *goo_stack = malloc(sizeof(char) * 1024 * 64);
	getcontext_ct(&goo_ctx);
	goo_ctx.stack.bp = goo_stack;
	goo_ctx.stack.sp = goo_stack;
	goo_ctx.stack.size = sizeof(char) * 1024 * 64;

	setcontext_ct(&foo_ctx);

	return EXIT_SUCCESS;
}