#include "aeco.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void *counter(void *arg)
{
	for (size_t i = 0; i < (size_t)arg; ++i) {
		printf("%ld\n", i);
		printf("---\n");
		co_yield();
	}
	return NULL;
}

int main(void)
{
	uintptr_t cos[4];

	for (size_t i = 0; i < sizeof(cos) / sizeof(uintptr_t); ++i)
		cos[i] = co_new(counter, (void *)(i + 2));

	while (true) {
		bool all_dead = true;

		for (size_t i = 0; i < sizeof(cos) / sizeof(uintptr_t); ++i)  {
			if (!co_is_dead(cos[i])) {
				all_dead = false;
				printf("cos[%ld] alive\n", i);
				printf("---\n");
				co_resume(cos[i]);
			}
		}

		if (all_dead)
			break;
	}

	for (size_t i = 0; i < sizeof(cos) / sizeof(uintptr_t); ++i)
		co_delete(cos[i]);
}
