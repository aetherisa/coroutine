# coroutine
A studying-purpose coroutine implementation based on C11/x86-64 assembly

# Features
* Create/destroy coroutines.
* Resume/yield to switch between caller/callee.
* Passing argument to coroutine and also get return value from it.

# Example
```
#include "coroutine.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void *counter(void *cnt)
{
	for (size_t i = 0; i < (size_t)cnt; ++i)
		printf("%ld\n", i);
	co_yield();
	for (size_t i = (size_t)cnt; i >= 1; --i)
		printf("%ld\n", i);

	return (void *)0;
}

int main(void)
{
	uintptr_t co = co_new(counter, (void *)2);

	while (!co_is_dead(co))
		co_resume(co);
	co_delete(co);
}
```

# TODO
* Add nested resume support.
* Add multi-thread support.
* Write actual unit tests.
