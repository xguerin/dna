#include <Private/PosixThreads.h>
#include <Private/Macros.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int pthread_create(pthread_t *thread, pthread_attr_t *attr, pthread_func_t start, void *arg)
{
  static int32_t index = 0;
  int32_t t_new;
  thread_info_t info;
  pthread_t new;
  char * default_name = "pthread";

  ASSERT_RETURN( (thread == NULL), EINVAL );

  new = malloc (sizeof(struct pthread));
  ASSERT_RETURN (!new, ENOMEM);

  new -> attributs = malloc (sizeof (pthread_attr_t));
  ASSERT_RETURN(!new->attributs, ENOMEM);

  new -> father = NULL;
  new -> children = 0;
  new -> cancel_type = PTHREAD_CANCEL_ASYNCHRONOUS;
  new -> cancel_state = PTHREAD_CANCEL_ENABLE;
  new -> cancel_bool = false;

  if (attr == NULL) {
    new -> attributs -> stacksize = 0x8000;
    new -> attributs -> detachstate = PTHREAD_CREATE_JOINABLE;
    new -> attributs -> schedinherited = PTHREAD_EXPLICIT_SCHED;
    new -> attributs -> schedpolicy = SCHED_FIFO;
    new -> attributs -> procid = PTHREAD_NOPROCID;
    new -> attributs -> name = (char *) malloc (32);

    sprintf (new -> attributs -> name, "%s_%ld", default_name, index ++);
  }
  else {
    memcpy (new -> attributs, attr, sizeof (pthread_attr_t));
    if (attr -> name == NULL)
     {
      new -> attributs -> name = (char *) malloc (32);
      sprintf (new -> attributs -> name, "%s_%ld", default_name, index ++);
    }
  }

  thread_create ((thread_handler_t)start, (void *) arg,
      new -> attributs -> name, new -> attributs -> procid, 0x8000, & t_new);

  thread_get_info (t_new, & info);

  new -> attributs -> stackaddr = NULL;
  new -> attributs -> stacksize = 0x8000;
  new -> tid = t_new;

  thread_resume (t_new);

  *thread = new;
  return 0;
}

