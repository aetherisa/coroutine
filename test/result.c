#include "coroutine.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void *foo(void *arg)
{
	return (void *)arg;
}

int main(void)
{
	uintptr_t co = co_new(foo, (void *)12345);
	co_resume(co);
	printf("foo return: %ld\n", (long)co_result(co));
}
