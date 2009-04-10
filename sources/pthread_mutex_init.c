#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	static int32_t index = 0;
	char name[32];

	sprintf (name, "pthread_mutex_%ld", index ++);
	mutex -> semaphore = core_component . semaphore . create (name, 1);

	switch (mutex -> semaphore) {
		case DNA_NO_MORE_SEM :
			return EAGAIN;
		case DNA_OUT_OF_MEM :
			return ENOMEM;
		default :
			return 0;
	}
}
