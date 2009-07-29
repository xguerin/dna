#include <Private/PosixThreads.h>

int pthread_cond_wait(pthread_cond_t *condition, pthread_mutex_t *mutex)
{
  status_t status = DNA_OK;

  condition -> count += 1;

  status = semaphore_linked_acquire (condition -> semaphore,
     mutex -> semaphore );
  if (status == DNA_BAD_SEM_ID) return EINVAL;

  pthread_mutex_lock (mutex);
  return 0;
}

