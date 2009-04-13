#include <private/pthread.h>

int pthread_cond_signal(pthread_cond_t *condition)
{
	status_t status = DNA_OK;

  if (condition -> count > 0)
  {
	  status = core_component . semaphore . release (condition -> semaphore, 1, 0);
	  if (status == DNA_BAD_SEM_ID) return EINVAL;

    condition -> count -= 1;
  }

	return 0;
}
