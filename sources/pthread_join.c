#include <pthread.h>
#include <stdio.h>

#include "private/macros.h"

int pthread_join(pthread_t thread, void **value_ptr)
{
	unsigned int status = 0;

	ASSERT_RETURN (thread == NULL, EINVAL);

	core_component . thread . wait (thread -> tid, (int32_t *) value_ptr);
	return status;
}
