/**
 * @file aeco.h
 * @brief minimal stackful coroutine primitives.
 *
 * argument is supported
 * return value is supported,
 *
 * scheduler is not supported,
 * multi-thread is not supported,
 * nested resumes is not supported.
 *
 * user are expected to store coroutine handles
 * and decide when to resume them.
 */

#ifndef AECO_COROUTINE_H
#define AECO_COROUTINE_H

#include <stdint.h>
#include <stdbool.h>

#define CO_STACK_SIZE (1ul << 14)

/**
 * @brief create a new coroutine.
 *
 * the coroutine will start by calling `func(arg)` when first resumed.
 * the returned value is an opaque coroutine handle.
 */
uintptr_t co_new(void *(*func)(void *), void *arg);

/**
 * @brief resume a coroutine.
 *
 * transfers execution to `co`. 
 * if the coroutine previously yielded,
 * it continues from the yield point.
 *
 * @pre `co` must not be dead.
 * @pre nested resume is not supported yet
 */
void co_resume(uintptr_t co);

/**
 * @brief yield from the current coroutine back to its caller.
 *
 * @pre must be called from inside a running coroutine.
 */
void co_yield(void);

/**
 * @brief get the return value of a finished coroutine.
 *
 * @pre `co_is_dead(co)` should be true.
 */
void *co_result(uintptr_t co);

/**
 * @brief destroy a coroutine and free its stack.
 *
 * @pre must not be called from inside the coroutine itself.
 */
void co_delete(uintptr_t co);

/**
 * @brief check whether a coroutine has finished.
 *
 * a coroutine becomes dead after its entry function returns.
 */
bool co_is_dead(uintptr_t co);

#endif // AECO_COROUTINE_H
