#include <pthread.h>

int pthread_cond_init(pthread_cond_t *condition, pthread_condattr_t *condattr)
{
	static int32_t index = 0;
	char name[32];

	sprintf (name, "pthread_cond_%ld", index ++);
  condition -> count = 0;
	condition -> semaphore = core_component . semaphore . create (name, 0);

	switch (condition -> semaphore) {
		case DNA_NO_MORE_SEM :
			return EAGAIN;
		case DNA_OUT_OF_MEM :
			return ENOMEM;
		default :
			return 0;
	}
}
