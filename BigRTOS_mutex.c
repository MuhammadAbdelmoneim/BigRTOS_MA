#include "BigRTOS_mutex.h"
#include <stddef.h>





void RTOS_mutexCreate(RTOS_mutex_t *pMutex , uint32_t mutexValue)
{
	/* Parameters Validation */
	if(pMutex == NULL)
	{
		/* Invaild parameter value */
	}
	else
	{
		/* Initialize mutex waiting list */
		RTOS_initList(&pMutex->pWaitingList);
		/* Initialize mutex value */
		pMutex->mutexValue = mutexValue;
	}
	
}


uint32_t RTOS_mutexLock(RTOS_mutex_t *pMutex , uint32_t waitingFlag)
{
	/* Variable to hold the status of the mutex */
	uint32_t mutexReturnStatus;
	/* Return Value */
	mutexReturnStatus = 0;
	/* Variable to used to end the while loop */
	uint32_t terminate = 0;
	/* pointer to the currently running thread */
	RTOS_TCB_t *pRunningThread;
	
	/* Parameters Validation */
	if((pMutex == NULL) && (waitingFlag != 0) && (waitingFlag != 1))
	{
		/*Invalid parameters */
	}
	else
	{
		while(1 != terminate)
		{
			/* ldrex r0 , [&pMutex->mutexValue] , so r0 will contain the mutex value */
			if(__LDREXW(&pMutex->mutexValue) == 1)  
			{
				/* The mutex is free so we need to lock it */
				/* Check if the mutex lock operation was interrupted */
				if(__STREXW(0,&pMutex->mutexValue) == 0)
				{
					/* Mutex is locked */
					/* Required before accessing protected resource */
					__DMB();
					/* Mutex lock succeeded, terminate the loop */
					terminate = 1;	
					/* return success */
					mutexReturnStatus = 1U;
				}
				else
				{
					/* Locking operation was interrupted */
				}
			}
			/* Mutex is locked */
			else
			{
				terminate = 1U;
				/* return success */
				mutexReturnStatus = 0U;
			}
			
		}
		/* if the Mutex is locked and we want to wait for it to be released nothing else to do \
		 * so we will add this thread to the mutex waiting list */
		if((waitingFlag == 1U) && (mutexReturnStatus == 0U))
		{
			/* Get the currently running thread */
			pRunningThread = RTOS_threadGetRunning();
			/* Remove the thread from */
			RTOS_removeThreadFromList(&pRunningThread->listItem);
			/* Add the thread to the waiting list of the mutex */
			RTOS_addThreadToSortedList(&pMutex->pWaitingList , &pRunningThread->listItem);
			/* Trigger context switch, set PendSV to pending */
			SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
			/* To return on the next try to the start of mutex lock function */
			mutexReturnStatus = 2;	
		}
		else
		{
			/* Erg3 ll user ytsarf hoa :D */
		}
	}
	return mutexReturnStatus;
	
}



uint32_t RTOS_mutexRelease(RTOS_mutex_t *pMutex)
{
	if(pMutex != NULL)
	{
		/* Required before releasing protected resource */
		__DMB();
		pMutex->mutexValue = 1U;
		
		if(pMutex->pWaitingList.numOfThreads > 0)
		{
			RTOS_TCB_t* pThread;
			pThread = (RTOS_TCB_t*)pMutex->pWaitingList.listEnd.pNext->pContainingTCB;
			if(pThread != NULL)
			{
				RTOS_removeThreadFromList(&pThread->listItem);
				
				RTOS_addThreadToReadyList(&pThread->listItem);
			}
			else
			{
				
			}
		}
	}
	else
	{
		/* NULL pointer Exception */
	}
}



