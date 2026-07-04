#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "aeco.h"

#define MMAP_SIZE ((CO_STACK_SIZE) << 1)
#define DEAD_FLAG (0x1)

static uintptr_t caller;

extern void forward_rax(void);
extern uintptr_t co_top(size_t);
extern void ctx_goto(uintptr_t);
extern void ctx_switch(uintptr_t *, uintptr_t);

static void co_exit(uintptr_t rax)
{
	uintptr_t top = co_top(CO_STACK_SIZE);

	*((uintptr_t *)top - 1) = DEAD_FLAG;
	*((uintptr_t *)top - 2) = rax;

	ctx_goto(caller);

	return;
}

uintptr_t co_new(void *(*func)(void *), void *arg)
{
	size_t page_size = sysconf(_SC_PAGESIZE);
	size_t stack_size = CO_STACK_SIZE;

	assert(stack_size > page_size &&
			"STACK_CAPACITY must be larger than _SC_PAGESIZE");

	void *raw = mmap(NULL,
			MMAP_SIZE,
			PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS,
			-1, 0);
	void *base = (void *)(((uintptr_t)raw + stack_size - 1) & ~(stack_size - 1));

	mprotect(base, page_size, PROT_NONE);

	void **top = (void **)((uintptr_t)base + stack_size);
	void **rsp = top - 3;

	*(--rsp) = co_exit;
	*(--rsp) = forward_rax;
	*(--rsp) = func;
	*(--rsp) = arg;		// push rdi
	*(--rsp) = 0; 		// push rbx
	*(--rsp) = 0;		// push rbp
	*(--rsp) = 0;		// push r12
	*(--rsp) = 0;		// push r13
	*(--rsp) = 0;		// push r14
	*(--rsp) = 0;		// push r15

	*(top - 1) = rsp;	// saved rsp
	*(top - 2) = 0;		// return value
	*(top - 3) = raw;	// raw
	
	return (uintptr_t)top;
}

void co_resume(uintptr_t co)
{
	uintptr_t rsp = *((uintptr_t *)co - 1);
	assert((rsp & DEAD_FLAG) == 0 &&
			"dead coroutine can't be resumed");
	ctx_switch(&caller, rsp);
}

void co_yield(void)
{
	uintptr_t top = co_top(CO_STACK_SIZE);
	ctx_switch((uintptr_t *)top - 1, caller);
}

void *co_result(uintptr_t co)
{
	return (void *)*((uintptr_t *)co - 2);
}

void co_delete(uintptr_t co)
{
	void *raw = (void *)*((uintptr_t *)co - 3);
	munmap(raw, MMAP_SIZE);
}

bool co_is_dead(uintptr_t co)
{
	return (*((uintptr_t *)co - 1) & 1) == 1;
}
