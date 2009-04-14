#include <malloc.h>
#include <Private/PosixThreads.h>
#include <Private/Macros.h>

extern int main (void);

int __libthread_start (void)
{
	int32_t my_id;
	thread_info_t cur;
	pthread_t wrapper = malloc (sizeof (struct pthread));

  my_id = thread_find (NULL);
	thread_get_info (my_id, & cur);

	wrapper -> attributs = malloc (sizeof (pthread_attr_t));
	wrapper -> father = NULL;
	wrapper -> children = 0;
	wrapper -> cancel_type = PTHREAD_CANCEL_ASYNCHRONOUS;
	wrapper -> cancel_state = PTHREAD_CANCEL_ENABLE;
	wrapper -> cancel_bool = false;
	wrapper -> tid = my_id;
	wrapper -> attributs -> stacksize = cur . stack . size;
	wrapper -> attributs -> stackaddr = (void *)cur . stack . base;
	wrapper -> attributs -> detachstate = PTHREAD_CREATE_JOINABLE;
	wrapper -> attributs -> schedinherited = PTHREAD_EXPLICIT_SCHED;
	wrapper -> attributs -> schedpolicy = SCHED_FIFO;
	wrapper -> attributs -> procid = cur . cpu_affinity;

	return main ();
}
