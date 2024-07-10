#include "../include/custom_ucontext.h"
#include <stdio.h>

// tests

ucontext_ct foo_ctx, goo_ctx, main_ctx;

void foo(void)
{
	puts("FOO: hello");
	swapcontext_ct(&foo_ctx, &goo_ctx);
	puts("FOO: bye");
	swapcontext_ct(&foo_ctx, &main_ctx);
}
void goo(void)
{
	puts("GOO: hello");
	swapcontext_ct(&goo_ctx, &foo_ctx);
	puts("GOO: bye");
	swapcontext_ct(&goo_ctx, &foo_ctx);
}

int main(void)
{
	main_ctx.stack.size = 1;
	getcontext_ct(&main_ctx);

	char *foo_stack = malloc(sizeof(char) * 1024 * 64);
	getcontext_ct(&foo_ctx);
	foo_ctx.stack.bp = foo_stack;
	foo_ctx.stack.sp = foo_stack;
	foo_ctx.stack.size = sizeof(sizeof(char) * 1024 * 64);
	makecontext_ct(&foo_ctx, &foo);

	char *goo_stack = malloc(sizeof(char) * 1024 * 64);
	getcontext_ct(&goo_ctx);
	goo_ctx.stack.bp = goo_stack;
	goo_ctx.stack.sp = goo_stack;
	goo_ctx.stack.size = sizeof(sizeof(char) * 1024 * 64);
	makecontext_ct(&goo_ctx, &goo);

	puts("MAIN: hello");
	swapcontext_ct(&main_ctx, &goo_ctx);
	puts("MAIN: bye");

	exit(EXIT_SUCCESS);
}