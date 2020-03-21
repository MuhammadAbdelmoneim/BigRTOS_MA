#ifndef BIG_RTOS_H_
#define BIG_RTOS_H_

/* File includes */
#include <stdint.h>


/* Global user Defined Values */
#define RTOS_THREAD_STACK_SIZE				 ((uint32_t)1024)		


/* Priorty levels are 0 to ( RTOS_NUM_PRIORTY_LEVELS - 1 )*/
#define RTOS_NUM_OF_PRIORTY_LEVELS 			 ((uint32_t)16)


/* CPU clock frequency in hertz */
#define SYSTEM_CORE_CLOCK_HZ                 ((uint32_t)16000000u)

/* System tick frequency in hertz */
#define SYS_TICK_RATE_HZ                     ((uint32_t)1000u)


#endif  //BIG_RTOS_H_

