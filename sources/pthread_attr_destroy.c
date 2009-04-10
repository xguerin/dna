#include <pthread.h>
#include <private/macros.h>

int pthread_attr_destroy(pthread_attr_t *attr)
{
	ASSERT_RETURN (!attr, EINVAL);

	return 0;
}
