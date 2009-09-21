#include <Private/PosixThreads.h>

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
  status_t status = DNA_OK;

  if (mutex -> lock == -1)
  {
    status = lock_create (& mutex -> lock);
    if (status != DNA_OK) return EINVAL;
  }

  lock_acquire (& mutex -> lock);

  mutex -> status = MUTEX_LOCKED;
  return 0;
}
