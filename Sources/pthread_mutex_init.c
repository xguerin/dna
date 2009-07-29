#include <Private/PosixThreads.h>

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
  static int32_t index = 0;
  char name[32];
  status_t status = DNA_OK;

  sprintf (name, "pthread_mutex_%ld", index ++);
  status = semaphore_create (name, 1, & mutex -> semaphore);

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
