#include <pthread.h>

#include "private/macros.h"

int pthread_attr_setdetachstate(pthread_attr_t *attr, int state)
{
	ASSERT_RETURN(!attr,EINVAL);

	attr->detachstate = state;
	return 0;
}
