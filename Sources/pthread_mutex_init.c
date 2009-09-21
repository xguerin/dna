#include <Private/PosixThreads.h>

int pthread_mutex_init (pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
  status_t status;

  status = lock_create (& mutex -> lock);
  if (status != DNA_OK) return EINVAL;

  return 0;
}
