#include "RTOS.h"
#include "BigRTOS_thread.h"
#include "stm32f407xx.h"
#include "RTOS_semaphore.h"

uint32_t svcEXEReturn;

static volatile uint32_t sysTickCounter = 0;


static RTOS_TCB_t idleThread;
static RTOS_stack_t idleThreadStack;

static void idleThreadFunction(void)
{
  while(1)
  {

  }
}


void RTOS_init(void)
{
	/* Disable interrupts */
	__disable_irq();

	
	
	/* Enable double word stack alignment */
	SCB->CCR |= SCB_CCR_STKALIGN_Msk;
	/* Configure and enable SysTick interrupts */
    SysTick_Config((SYSTEM_CORE_CLOCK_HZ / SYS_TICK_RATE_HZ));
	/* Set priority group to 3
    * bits[3:0] are the sub-priority,
    * bits[7:4] are the preempt priority (0-15) */
    NVIC_SetPriorityGrouping(3);

    /* Set priority levels */
    NVIC_SetPriority(SVCall_IRQn, 0);
    NVIC_SetPriority(SysTick_IRQn, 1);

    /* Set PendSV to lowest possible priority */
    NVIC_SetPriority(PendSV_IRQn, 0xFF);

    /* Enable SVC and PendSV interrupts */
    NVIC_EnableIRQ(PendSV_IRQn);
    NVIC_EnableIRQ(SVCall_IRQn);

    /* Disable all interrupts except SVC */
    __set_BASEPRI(1);

    /* Initialize thread module */
    RTOS_threadInitList();

  /* Enable interrupts */
  __enable_irq();	
}

void RTOS_SVC_Handler_main(uint32_t *svc_args)
{
	uint32_t returnStatus;
	uint8_t SVC_number;
	/* Will get the last instruction when we was interrupted */
	SVC_number = ((char*)svc_args[6]) [-2];
	switch (SVC_number)
	{
		case 0 : 
			RTOS_schedulerStart();
			break;
		case 1 : 
			RTOS_threadCreate((RTOS_TCB_t*)svc_args[0],
							  (RTOS_stack_t*)svc_args[1],
							  (uint32_t)svc_args[2],
							  (void*)svc_args[3]);
			break;
		case 2 : 
			RTOS_mutexCreate((RTOS_mutex_t*)svc_args[0],(uint32_t)svc_args[1]);
		    break;
		case 3 : 
			returnStatus = RTOS_mutexLock((RTOS_mutex_t*)svc_args[0],(uint32_t)svc_args[1]);
			break;
		case 4 : 
			RTOS_mutexRelease((RTOS_mutex_t*)svc_args[0]);
			break;
		case 5 :
			RTOS_semaphoreCreate((RTOS_semaphore_t*) svc_args[0], (uint32_t) svc_args[1]);
			break;
		case 6 : 
			returnStatus = RTOS_semaphoreTake((RTOS_semaphore_t*) svc_args[0], (uint32_t) svc_args[1]);
			break;
		case 7 :
			RTOS_semaphoreGive((RTOS_semaphore_t*) svc_args[0]);
			break;
		default : 
			/* Nothing to do in default case */
		    break;		
	}
	switch(SVC_number)
	{
		case 3 :
		case 6 :
			/* We didn't get the mutex and we want to wait */
			if(returnStatus == 2)
			{
				/* svc_args[6] = pc -> so we store in the pc the pc - 2 (the instruction before) to start from the start of Mutex lock function */
				svc_args[6] = svc_args[6] - 2;
			}
			else
			{
				/* The return status to the user svc_args = R0 */
				svc_args[0] = returnStatus;
			}
			break;
	}
		
	
}



/* This function will start the scheduler */
void RTOS_schedulerStart(void)
{
	/* Create the idel thread */
	RTOS_threadCreate(&idleThread ,&idleThreadStack, RTOS_NUM_OF_PRIORTY_LEVELS - 1, idleThreadFunction); 
	
	RTOS_TCB_t *pRunningThread;
	
	RTOS_threadSwitchRunning();
	
	pRunningThread = RTOS_threadGetRunning();
	/* Set SVC interrupt return to the first thread */
	svcEXEReturn = MEM32_ADDRESS(pRunningThread->pStackPointer);
	/* Return to thread with PSP */
    __set_PSP((uint32_t)(pRunningThread->pStackPointer + 10 * 4));

    /* Switch to use Process Stack, unprivileged state */
    __set_CONTROL(MEM32_ADDRESS((pRunningThread->pStackPointer + (1 << 2))));

    /* Execute ISB after changing CONTROL */
    __ISB();

    /* Reset tick counter */
    sysTickCounter = 0;

    /* Enable all interrupts */
    __set_BASEPRI(0);
	
}



void SysTick_Handler(void)
{
  /* Trigger context switch, set PendSV to pending */
  SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
  /* Increment SysTick counter */
  ++sysTickCounter;
}
