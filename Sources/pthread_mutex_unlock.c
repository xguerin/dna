#include <Private/PosixThreads.h>

int pthread_mutex_unlock (pthread_mutex_t *mutex)
{
  lock_release (& mutex -> lock);
  return 0;
}
