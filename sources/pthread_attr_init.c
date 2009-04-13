#include <string.h>
#include <private/pthread.h>
#include <private/macros.h>

int pthread_attr_init(pthread_attr_t *attr)
{
	ASSERT_RETURN(!attr,EINVAL);

	memset(attr, 0, sizeof(*attr));

	attr->detachstate    = PTHREAD_CREATE_JOINABLE;
	attr->schedpolicy    = SCHED_FIFO;
	attr->schedinherited = PTHREAD_EXPLICIT_SCHED;
	attr->procid         = PTHREAD_NOPROCID;

	return 0;
}
