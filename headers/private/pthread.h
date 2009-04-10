#ifndef _PTHREAD_P_H
#define _PTHREAD_P_H

#include <pthread/pthread.h>

typedef struct pthread_handler_rec {
	void (*func)(void *);
	void *argf;
} _pthread_handler_rec;

#endif 
