/*
 * Frédéric Pétrot
 * $Log: semaphoreP.h,v $
 * Revision 1.2  2006/12/08 22:27:49  gerin
 * Externalize Hardware Abstraction Layer for mutek.
 *
 * Revision 1.5  2005/04/21 13:51:28  gomez
 * - add double chained list in mutex and semaphore waiting list
 * - remove some static inline declaration
 * - add a mode with activation = deadline in real-time
 * - remove true malloc/free in signal for real-time system,
 *   replace it by a rolling buffer allocation
 *
 * Revision 1.4  2003/12/01 09:27:27  fred
 * Adding the spin lock function of the pthread, as introduced in the
 * 2000 version of the standard.
 * Changing the internal sched_sem_t type into pthread_spinlock_t since
 * it indeed replaces it.
 * Still using SEM_LOCK and SEM_UNLOCK in the scheduler, but now the user
 * is able to use the pthread stuffs on spin locks if he wants to.
 *
 * Revision 1.3  2003/07/01 12:28:01  gomez
 * make the system configurable thru external variables instead of define
 *
 * Revision 1.2  2003/05/07 13:18:05  gomez
 * Modification of support to support Real time with two
 * architectures: SMP or distributed scheduling.
 * Add pthread barrier, longjmp, event management and cleanup handler.
 * Add real time ss1 and hs1 communication.
 *
 * Revision 1.1.1.1  2002/02/28 12:58:55  disydent
 * Creation of Disydent CVS Tree
 *
 * Revision 1.1.1.1  2001/11/19 16:55:46  pwet
 * Changing the CVS tree structure of disydent
 *
 * Revision 1.2  2001/09/17 14:50:38  pwet
 * New version of the pthreads : All core_x functions are assumed to be
 * executed in critical section.
 * Therefore, interrupt masking and unmasking is performed in the pthread_x
 * functions
 *
 * $Id: semaphoreP.h,v 1.2 2006/12/08 22:27:49 gerin Exp $
 */

#ifndef SEMAPHOREP_H
#define SEMAPHOREP_H

#include "pthreadP.h"

typedef struct sem_t {
   pthread_spinlock_t sem;
   pthread_t          wait;
   pthread_t          end;
   int                val;
} sem_t;

#endif
