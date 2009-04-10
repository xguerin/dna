/*
   signalP.h: signal library for mutek
   implementation : Pascal Gomez
*/
/*
 * $Log: signalP.h,v $
 * Revision 1.2  2006/12/08 22:27:49  gerin
 * Externalize Hardware Abstraction Layer for mutek.
 *
 * Revision 1.3  2005/04/21 13:51:28  gomez
 * - add double chained list in mutex and semaphore waiting list
 * - remove some static inline declaration
 * - add a mode with activation = deadline in real-time
 * - remove true malloc/free in signal for real-time system,
 *   replace it by a rolling buffer allocation
 *
 * Revision 1.2  2003/12/01 09:27:27  fred
 * Adding the spin lock function of the pthread, as introduced in the
 * 2000 version of the standard.
 * Changing the internal sched_sem_t type into pthread_spinlock_t since
 * it indeed replaces it.
 * Still using SEM_LOCK and SEM_UNLOCK in the scheduler, but now the user
 * is able to use the pthread stuffs on spin locks if he wants to.
 *
 * Revision 1.1  2003/05/07 13:18:05  gomez
 * Modification of support to support Real time with two
 * architectures: SMP or distributed scheduling.
 * Add pthread barrier, longjmp, event management and cleanup handler.
 * Add real time ss1 and hs1 communication.
 *
 * $Id: signalP.h,v 1.2 2006/12/08 22:27:49 gerin Exp $
 */

#ifndef _SIGNALP_H
#define _SIGNALP_H

#include <stdio.h>
#include <stdlib.h>
#include <mutek_hal.h>
#include "mutek.h"
#if USE_LONGJMP
#include "setjmpP.h"
#endif
/* defered event : no physical interrupt beetween processor, and/or
 with timer. Signal management is done when calling scheduler (in pthread
 functions) 
*/

#define SIGNAL_NUMBER 9
#define SIGUSER1 0
#define SIGKER1 1
#define SIGKER2 2
#define SIGKEXT1 3
#define SIGKEXT2 4
#define SIGKEXT3 5
#define SIGKEXT4 6
#define SIGKEXT5 7
#define SIGKEXT6 8

#define MAX_SCHEDULER_SIGNAL_NUMBER 4
#define MAX_TASK_SIGNAL_NUMBER 3

typedef struct event{
   struct event *next;
   struct event *previous;
   struct pthread *target;
   int id;
   int date;
   int status;
}event_t;

typedef struct signal{
   struct signal *next;
   struct pthread *owner;
   int id;
   int mask;
   void (*handler)(void *);
   void *arg;
}signal_t;

extern void preemptionHandler(void);
extern int postedsignal[SIGNAL_NUMBER];
extern signal_t *schedsig[SIGNAL_NUMBER];
extern pthread_spinlock_t sigsem;
#endif
