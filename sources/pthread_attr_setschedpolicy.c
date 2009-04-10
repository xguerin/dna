#include <pthread.h>

#include "private/macros.h"

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
	ASSERT_RETURN(!attr,EINVAL);

	attr->schedpolicy = policy;
	return 0;
}
