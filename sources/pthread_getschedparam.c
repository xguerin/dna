#include <string.h>
#include <private/pthread.h>
#include <private/macros.h>

int pthread_getschedparam(pthread_t t, int *p, struct sched_param *sp)
{
	ASSERT_RETURN (!t || !sp, EINVAL);

	*p = t -> attributs -> schedpolicy;
	memcpy (sp, & t -> attributs -> schedparam, sizeof (*sp));
	return 0;
}

