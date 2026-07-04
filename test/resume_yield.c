#include "aeco.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void *counter(void *cnt)
{
	for (size_t i = 0; i < (size_t)cnt; ++i)
		printf("%ld\n", i);
	printf("counter: first complete\n");
	co_yield();
	for (size_t i = (size_t)cnt; i >= 1; --i)
		printf("%ld\n", i);
	printf("counter: second complete\n");

	return (void *)0;
}

int main(void)
{
	uintptr_t co = co_new(counter, (void *)2);
	printf("main: first resume\n");
	co_resume(co);
	printf("main: second resume\n");
	co_resume(co);
}
