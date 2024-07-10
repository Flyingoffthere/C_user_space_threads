#include <stdio.h>
#include "../include/custom_ucontext.h"

#define TRUE 1
#define FALSE 0

#define USE_INTEL_SYNTAX TRUE
#define USE_GCC_BUILTINS FALSE

void getcontext_ct(ucontext_ct *ucontext)
{
	asm volatile(
		"mov %0, rbp\n\t" 
		: "=m"(ucontext->stack.sp)
		:
		);
	ucontext->stack.sp += RSP_BEFORE_CALL_OFFSET;

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
		: "=m"(ucontext->mcontext.rax), "=m"(ucontext->mcontext.rcx),
		  "=m"(ucontext->mcontext.rdx), "=m"(ucontext->mcontext.rsi),
		  "=m"(ucontext->mcontext.rdi), "=m"(ucontext->mcontext.r8),
		  "=m"(ucontext->mcontext.r9),  "=m"(ucontext->mcontext.r10),
		  "=m"(ucontext->mcontext.r11)
		:
		:
		);

#if USE_GCC_BUILTINS
	(ucontext)->mcontext.rip = (uintptr_t)__builtin_return_address(0);
#else
	asm volatile(
		"mov rax, [rbp + " RETURN_INSTRUCTION_OFFSET "]\n\t"
		"mov %0, rax\n\t" 
		: "=r"(ucontext->mcontext.rip)
		:
		: "rax"
		);
#endif

	asm volatile(
		"mov %0, [rbp]\n\t" 
		: "=r"(ucontext->stack.bp)
		:
		);
}

void setcontext_ct(const ucontext_ct *ucontext)
{
	static uintptr_t NEXT_RIP;

	if (ucontext->stack.size == 0)
	{
		fprintf(stderr, "[setcontext_ct failed]: stack not allocated\n");
		return;
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
}

void swapcontext_ct(ucontext_ct *oucp, const ucontext_ct *ucp)
{
	getcontext_ct(oucp);

	asm volatile(
		"mov rax, [rbp + 8]\n\t"
		"mov %0, rax\n\t" 
		: "=r"(oucp->mcontext.rip)
		:
		: "rax"
		); // correction, later need to fix

	setcontext_ct(ucp);
}

void makecontext_ct(ucontext_ct *ucp, void (*func)(void))
{
	ucp->mcontext.rip = (uintptr_t)func;
}