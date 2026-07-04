#ifndef AETHERIS_COROUTINE_H
#define AETHERIS_COROUTINE_H

#include <stdint.h>
#include <stdbool.h>

#define STACK_SIZE (1ul << 14)

uintptr_t co_new(void *(*func)(void *), void *arg);
void co_resume(uintptr_t co);
void co_yield(void);
void *co_result(uintptr_t co);
void co_delete(uintptr_t co);
bool co_is_dead(uintptr_t co);

#endif // AETHERIS_COROUTINE_H
