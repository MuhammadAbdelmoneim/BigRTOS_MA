#ifndef RTOS_H_
#define RTOS_H_
#include <stdint.h>
#include "BigRTOS_thread.h"
#include "BigRTOS_mutex.h"
#include "RTOS_semaphore.h"


void RTOS_init(void);

void RTOS_ThreadCreate(RTOS_TCB_t *pTCB ,RTOS_stack_t *pThreadStack ,uint32_t priorty, void *pThreadFunction);

void RTOS_SVC_Handler_main(uint32_t * svc_args);

void RTOS_schedulerStart(void);

#endif //RTOS_SVC_Handler_main
