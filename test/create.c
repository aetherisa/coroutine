#include "aeco.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void *counter(void *cnt)
{
	for (size_t i = 0; i < (size_t)cnt; ++i)
		printf("%ld\n", i);

	return (void *)0;
}

int main(void)
{
	uintptr_t co = co_new(counter, (void *)5);

	printf("%p", (void *)co);
}
