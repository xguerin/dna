#include <pthread.h>

#include "private/macros.h"

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *state)
{
	ASSERT_RETURN (!attr || !state,EINVAL);
	*state = attr -> detachstate;

	return 0;
}


