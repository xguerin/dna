#include <Private/PosixThreads.h>

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	status_t status = DNA_OK;

	if (mutex -> semaphore == -1) {
		status = semaphore_create ("pthread_mutex_static", 1, & mutex -> semaphore);
	}
	else status = semaphore_acquire (mutex -> semaphore, 1, 0, -1);

  if (status != DNA_OK)
  {
    switch (status)
    {
      case DNA_NO_MORE_SEM :
        return EAGAIN;
      case DNA_OUT_OF_MEM :
        return ENOMEM;
      default :
        return EINVAL;
    }
  }

  return 0;
}
