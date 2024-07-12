#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../include/c_ucontext.h"
#include "../include/dstructs/stack.h"

#define TRUE 1
#define FALSE 0

int getcontext_ct(ucontext_ct *ucontext)
{
	asm volatile(
		"mov %0, rax\n\t"
		"mov %1, rcx\n\t"
		"mov %2, rdx\n\t"
		"mov %3, rsi\n\t"
		"mov %4, rdi\n\t"
		"mov %5, r8\n\t"
		"mov %6, r9\n\t"
		"mov %7, r10\n\t"
		"mov %8, r11\n\t" 
		"lea r12, [rbp + " RSP_BEFORE_CALL_OFFSET "]\n\t"
		"mov %9, r12\n\t"
		"mov r12, [rbp + " RETURN_INSTRUCTION_OFFSET "]\n\t"
		"mov %10, r12\n\t" 
		"mov r12, [rbp]\n\t" 
		"mov %11, r12\n\t" 
		: "=m"(ucontext->mcontext.rax), "=m"(ucontext->mcontext.rcx),
		  "=m"(ucontext->mcontext.rdx), "=m"(ucontext->mcontext.rsi),
		  "=m"(ucontext->mcontext.rdi), "=m"(ucontext->mcontext.r8),
		  "=m"(ucontext->mcontext.r9),  "=m"(ucontext->mcontext.r10),
		  "=m"(ucontext->mcontext.r11), "=m"(ucontext->stack.sp),
		  "=m"(ucontext->mcontext.rip), "=m"(ucontext->stack.bp)
		:
		: "r12"
		);

	return EXIT_SUCCESS; 
}

static void *current_args;

int setcontext_ct(const ucontext_ct *ucontext)
{
	static uintptr_t NEXT_RIP;

	current_args = ucontext->args;

	if (ucontext->stack.size == 0)
	{
		fprintf(stderr, "[setcontext_ct failed]: stack not allocated\n");
		return EXIT_FAILURE;
	}

	NEXT_RIP = ucontext->mcontext.rip;

	asm volatile (
		"mov rax, %0\n\t"
		"mov rcx, %1\n\t"
		"mov rdx, %2\n\t"
		"mov rsi, %3\n\t"
		"mov rdi, %4\n\t"
		"mov r8, %5\n\t"
		"mov r9, %6\n\t"
		"mov r10, %7\n\t"
		"mov r11, %8\n\t"
		"mov rsp, %9\n\t"
		"mov rbp, %10\n\t"
		"jmp %11"
		:
		: "g"(ucontext->mcontext.rax), "g"(ucontext->mcontext.rcx),
		  "g"(ucontext->mcontext.rdx), "g"(ucontext->mcontext.rsi),
		  "g"(ucontext->mcontext.rdi), "g"(ucontext->mcontext.r8),
		  "g"(ucontext->mcontext.r9),  "g"(ucontext->mcontext.r10),
		  "g"(ucontext->mcontext.r11), "g"(ucontext->stack.sp),
		  "g"(ucontext->stack.bp),	   "r"(NEXT_RIP)
		: "rax", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10", "r11"
	);

	return EXIT_FAILURE;
}

int check_enough_stack_space_left(const stack_ct *stack)
{
	size_t space_occupied = (uintptr_t) stack->bp - (uintptr_t) stack->sp;
	intmax_t space_left = stack->size - space_occupied;
	if (space_left < STACK_ALIGNMENT) return FALSE;
	return TRUE;
}

int swapcontext_ct(ucontext_ct *oucp, const ucontext_ct *ucp)
{
	getcontext_ct(oucp);

	asm volatile(
		"lea r12, [rbp + " RSP_BEFORE_CALL_OFFSET "]\n\t"
		"mov %0, r12\n\t"
		"mov r12, [rbp + " RETURN_INSTRUCTION_OFFSET "]\n\t"
		"mov %1, r12\n\t" 
		"mov r12, [rbp]\n\t" 
		"mov %2, r12\n\t"  
		: "=m"(oucp->stack.sp), "=m"(oucp->mcontext.rip),
		  "=m"(oucp->stack.bp)
		:
		: "r12"
		);

	if (check_enough_stack_space_left(&ucp->stack)) {
			setcontext_ct(ucp);
			return EXIT_SUCCESS;
	}
	errno = ENOMEM;

	return EXIT_FAILURE;
}

static stack ucontext_stack = {
	.size = 0,
	.comparator = NULL,
	.data_destructor = NULL,
	.head = NULL,
	.tail = NULL,
};

void restore_linked(void)
{
	static const ucontext_ct *linked_context;
	if (!linked_context || stack_pop(&ucontext_stack, (void **) &linked_context) == -1) 
		exit(EXIT_SUCCESS);
	setcontext_ct(linked_context);
}

void makecontext_ct(ucontext_ct *ucp, routine worker, void *args)
{
	ucp->mcontext.rip = (uintptr_t) worker;
	ucp->args = args;

	if (stack_push(&ucontext_stack, ucp->uc_link) == -1) {
		errno = ENOMEM;
		exit(EXIT_FAILURE);
	}
	*((uintptr_t *)ucp->stack.bp) = (uintptr_t) restore_linked;
	ucp->stack.bp += sizeof(uintptr_t);
}