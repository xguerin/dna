#include <Private/PosixThreads.h>

int pthread_cond_wait (pthread_cond_t *condition, pthread_mutex_t *mutex)
{
  status_t status = DNA_OK;

  condition -> count += 1;
  status = semaphore_acquire (condition -> semaphore, 1, 0, 0);

  if (status == DNA_WOULD_BLOCK)
  {
    pthread_mutex_unlock (mutex);
    status = semaphore_acquire (condition -> semaphore, 1, 0, -1);
    pthread_mutex_lock (mutex);
  }

  return 0;
}

