#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>

typedef int exit_status; 
typedef void(*routine)(void);

#ifdef __x86_64__

#define STACK_ALIGNMENT 8
#define RETURN_INSTRUCTION_OFFSET "8"

typedef struct mcontext_
{
	volatile uintptr_t rax;
	volatile uintptr_t rcx;
	volatile uintptr_t rdx;
	volatile uintptr_t rsi;
	volatile uintptr_t rdi;
	volatile uintptr_t r8;
	volatile uintptr_t r9;
	volatile uintptr_t r10;
	volatile uintptr_t r11;
	volatile uintptr_t rip;
} mcontext_ct;

#else
#error "incorrect machine"
#endif

typedef struct stack_
{
	void *bp;
	void *sp;
	size_t size;
} stack_ct;

typedef struct ucontext_
{
	struct ucontext_ *uc_link;
	stack_ct stack;
	mcontext_ct mcontext;
	void *args;
} ucontext_ct;

exit_status getcontext_ct(ucontext_ct *);
exit_status setcontext_ct(const ucontext_ct *);
void makecontext_ct(ucontext_ct *, routine, void *args);
exit_status swapcontext_ct(ucontext_ct *, const ucontext_ct *);