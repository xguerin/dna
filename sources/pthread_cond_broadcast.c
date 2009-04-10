#include <pthread.h>
#include <cpu.h>

#include "private/macros.h"

int pthread_cond_broadcast(pthread_cond_t *condition)
{
	status_t status = DNA_OK;

  if (condition -> count > 0)
  {
	  status = core_component . semaphore . release (condition -> semaphore,
        condition -> count, 0);
	  if (status == DNA_BAD_SEM_ID) return EINVAL;

    condition -> count = 0;
  }

	return 0;
}
