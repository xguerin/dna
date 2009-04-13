#include <private/pthread.h>
#include <private/macros.h>

int pthread_attr_setstackaddr(pthread_attr_t *attr, void *addr)
{
	ASSERT_RETURN(!attr || addr == NULL,EINVAL);

	attr->stackaddr = addr;
	return 0;
}
