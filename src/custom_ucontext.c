#include "../include/custom_ucontext.h"

#define TRUE 1
#define FALSE 0

#define USE_INTEL_SYNTAX TRUE
#define USE_GCC_BUILTINS FALSE

#if USE_INTEL_SYNTAX
#define SYNTAX_SPEC ".intel_syntax noprefix\n\t"
#define SYNTAX_UNSPEC ".att_syntax noprefix\n\t"
#else
#define SYNTAX_SPEC ".att_syntax noprefix\n\t"
#define SYNTAX_UNSPEC ""
#endif

void getcontext_ct(ucontext_ct *ucontext)
{
	uintptr_t rsp;
	asm volatile(
		SYNTAX_SPEC
		"mov %0, rbp\n\t" SYNTAX_UNSPEC
		: "=r"(rsp)
		:);
	rsp += STACK_ALIGNMENT;
	asm volatile(
		SYNTAX_SPEC
		"mov %0, rax\n\t"
		"mov %1, rcx\n\t"
		"mov %2, rdx\n\t"
		"mov %3, rsi\n\t"
		"mov %4, rdi\n\t"
		"mov %5, r8\n\t"
		"mov %6, r9\n\t"
		"mov %7, r10\n\t"
		"mov %8, r11\n\t" SYNTAX_UNSPEC
		: "=r"(ucontext->mcontext.rax), "=r"(ucontext->mcontext.rcx),
		  "=r"(ucontext->mcontext.rdx), "=r"(ucontext->mcontext.rsi),
		  "=r"(ucontext->mcontext.rdi), "=r"(ucontext->mcontext.r8),
		  "=r"(ucontext->mcontext.r9), "=r"(ucontext->mcontext.r10),
		  "=r"(ucontext->mcontext.r11)
		:
		:);
#if USE_GCC_BUILTINS
	(ucontext)->mcontext.rip = (uintptr_t)__builtin_return_address(0);
#else
	asm volatile(
		SYNTAX_SPEC
		"mov rax, [rbp + 8]\n\t"
		"mov %0, rax\n\t" SYNTAX_UNSPEC
		: "=r"(ucontext->mcontext.rip)
		:
		: "rax");
#endif
	asm volatile(
		SYNTAX_SPEC
		"mov %0, [rbp]\n\t" SYNTAX_UNSPEC
		: "=r"(ucontext->stack.sbp)
		:);
	ucontext->stack.size = (uintptr_t)ucontext->stack.sbp - rsp;
}

size_t next_rip;

void setcontext_ct(const ucontext_ct *ucontext)
{
	next_rip = ucontext->mcontext.rip;
	uintptr_t rsp = (uintptr_t)ucontext->stack.sbp - ucontext->stack.size;

	asm volatile(
		SYNTAX_SPEC
		"mov rax, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(ucontext->mcontext.rax)
		: "rax");

	asm volatile(
		SYNTAX_SPEC
		"mov rcx, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(ucontext->mcontext.rcx)
		: "rcx");

	asm volatile(
		SYNTAX_SPEC
		"mov rdx, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(ucontext->mcontext.rdx)
		: "rdx");

	asm volatile(
		SYNTAX_SPEC
		"mov rsi, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(ucontext->mcontext.rsi)
		: "rsi");

	asm volatile(
		SYNTAX_SPEC
		"mov rdi, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(ucontext->mcontext.rdi)
		: "rdi");

	asm volatile(
		SYNTAX_SPEC
		"mov r8, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(ucontext->mcontext.r8)
		: "r8");

	asm volatile(
		SYNTAX_SPEC
		"mov r9, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(ucontext->mcontext.r9)
		: "r9");

	asm volatile(
		SYNTAX_SPEC
		"mov r10, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(ucontext->mcontext.r10)
		: "r10");

	asm volatile(
		SYNTAX_SPEC
		"mov r11, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(ucontext->mcontext.r11)
		: "r11");

	asm volatile(
		SYNTAX_SPEC
		"mov rsp, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(rsp)
		:);

	asm volatile(
		SYNTAX_SPEC
		"mov rbp, %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(ucontext->stack.sbp)
		:);

	asm volatile(
		SYNTAX_SPEC
		"jmp %0\n\t" SYNTAX_UNSPEC
		:
		: "r"(next_rip)
		:);
}

inline __attribute__((always_inline)) void swapcontext_ct(ucontext_ct *oucp, const ucontext_ct *ucp)
{
	getcontext_ct(oucp);
	setcontext_ct(ucp);
}

void makecontext_ct(ucontext_ct *ucp, void (*func)(void))
{
	ucp->mcontext.rip = (uintptr_t)func + FUNCTION_OFFSET;
}