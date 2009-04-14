#include <Private/PosixThreads.h>

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	status_t status = DNA_OK;

	status = semaphore_release (mutex -> semaphore, 1, 0);

	if (status == DNA_BAD_SEM_ID) return EINVAL;
	else return 0;
}
