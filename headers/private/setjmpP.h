/* setjmpP header for mutek */
/*Author: Pascal Gomez
 * $Log: setjmpP.h,v $
 * Revision 1.2  2006/12/08 22:27:49  gerin
 * Externalize Hardware Abstraction Layer for mutek.
 *
 * Revision 1.1  2003/05/07 13:18:05  gomez
 * Modification of support to support Real time with two
 * architectures: SMP or distributed scheduling.
 * Add pthread barrier, longjmp, event management and cleanup handler.
 * Add real time ss1 and hs1 communication.
 *
 * $Id: setjmpP.h,v 1.2 2006/12/08 22:27:49 gerin Exp $
 */
/* Definition of the setjmp/longjmp jmp_buf
*/

#ifndef _SETJMPP_H_
#define _SETJMPP_H_
#include <mutek_hal.h>
#include "mutek.h"

/*define jmp_buf to save context*/
typedef struct jmp_buf_t {
         HAL_SavedRegisters reg;
         unsigned int val;
         struct pthread *owner;
}jmp_buf_t;
#if 0
typedef unsigned int jmp_buf[NUMBER_OF_REGISTERS + 2];
#endif
extern void core_longjmp(jmp_buf, int);
extern int core_setjmp(jmp_buf);

#endif
