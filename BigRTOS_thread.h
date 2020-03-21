#ifndef 	BIGRTOS_THREAD_H_
#define 	BIGRTOS_THREAD_H_

/* Files includes */
#include <stdint.h>
#include "BigRTOS_config.h"
#include "common_macros.h"
#include "BigRTOS_list.h"
#include "stm32f407xx.h"

/* Thread Control Block definition */
typedef struct RTOS_TCB{
	uint32_t pStackPointer;
	/*Used to chain threads */
	RTOS_list_item_t listItem;
	unsigned int threadID;
	unsigned int threadPriority;
	void * pThreadFun;
}RTOS_TCB_t;


typedef struct RTOS_stack{
	uint64_t stack[RTOS_THREAD_STACK_SIZE];
}RTOS_stack_t;


void RTOS_threadInitList(void);

void RTOS_threadCreate(RTOS_TCB_t *pTCB ,RTOS_stack_t *pThreadStack ,uint32_t priorty, void *pThreadFunction);


void RTOS_threadSwitchRunning(void);

RTOS_TCB_t *RTOS_threadGetRunning(void);

void RTOS_addThreadToReadyList(RTOS_list_item_t *pNewItem);

#endif //BIGRTOS_THREAD_H_
