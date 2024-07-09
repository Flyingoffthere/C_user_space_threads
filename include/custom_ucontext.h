#pragma once
#include <stdint.h>
#include <stdlib.h>

#ifdef __x86_64__
typedef struct mcontext_
{
	volatile size_t rax;
	volatile size_t rcx;
	volatile size_t rdx;
	volatile size_t rsi;
	volatile size_t rdi;
	volatile size_t r8;
	volatile size_t r9;
	volatile size_t r10;
	volatile size_t r11;
	volatile size_t rip;
} mcontext_ct;

#else
#error "incorrect machine"
#endif

typedef struct stack_
{
	void *sbp;
	size_t size;
} stack_ct;

typedef struct ucontext_
{
	struct ucontext_ *uc_link;
	stack_ct stack;
	mcontext_ct mcontext;
} ucontext_ct;

void getcontext_ct(ucontext_ct *);
void setcontext_ct(const ucontext_ct *);
