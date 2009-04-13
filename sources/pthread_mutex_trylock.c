#include <private/pthread.h>

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	status_t status = DNA_OK;

	status = core_component . semaphore . acquire (mutex -> semaphore, 0, 0);

	if (status == DNA_BAD_SEM_ID) return EINVAL;
	else if (status == DNA_WOULD_BLOCK) return EBUSY;
	else return 0;
}
