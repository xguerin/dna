#include <Private/PosixThreads.h>

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	status_t status = DNA_OK;

	if (mutex -> semaphore == -1) {
		mutex -> semaphore = semaphore_create ("pthread_mutex_static", 1);
		status = mutex -> semaphore;
	}
	else status = semaphore_acquire (mutex -> semaphore, 0, -1);

	if (status == DNA_BAD_SEM_ID) return EINVAL;
	else if (status == DNA_NO_MORE_SEM) return ENOMEM;
	else return 0;
}
