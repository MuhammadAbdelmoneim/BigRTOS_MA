#include "RTOS_semaphore.h"
#include "BigRTOS_thread.h"

/*
* @brief :
* @param :
* @retVal :
*/
void RTOS_semaphoreCreate(RTOS_semaphore_t *pSemaphore , uint32_t semaphoreValue)
{
	if(pSemaphore != NULL && semaphoreValue != 0U)
	{
		/* Initialize the semaphore */
		pSemaphore->semaphoreValue = semaphoreValue;
		
		RTOS_initList(&pSemaphore->waitingList);
	}
	else
	{
		/* NULL pointer */
	}
}

/*
* @brief :
* @param :
* @retVal :
*/
uint32_t RTOS_semaphoreTake(RTOS_semaphore_t*pSemaphore , uint32_t waitingFlag)
{
	uint32_t returnStatus;
	
	returnStatus = 0U;
	
	uint32_t terminate = 0U;
	
	uint32_t semaphoreTemp;
	
	while(1U != terminate)
	{
		semaphoreTemp = __LDREXW(&pSemaphore->semaphoreValue);
		/* Check the semaphore value */
		if(semaphoreTemp > 0U)
		{
			if(0U == __STREXW((semaphoreTemp - 1),&pSemaphore->semaphoreValue) )
			{
				/* Semaphore was aquired successfully */
				/* Required before accessing protected resource */
				__DMB();
				/* Return Success */
				returnStatus = 1U;
				/* Terminate the loop */
				terminate = 1U;
			}
			else
			{
				/* Something interrupted the write operation for the semaphore */
			}
		}
		/* Semaphore is not available */
		else
		{
			returnStatus = 0U;
			/* Terminate the loop */
			terminate = 1U;
		}
	}
	/* Check if the user want to wait for the semaphore */
	if((1U != returnStatus) && (1U == waitingFlag))
	{
		/* pointer to the current running thread TCB */
		RTOS_TCB_t * pRunningThread;
		/* Get the current running thread */
		pRunningThread = RTOS_threadGetRunning();
		/* Remove the current thread from the ready list */
		RTOS_removeThreadFromList(&pRunningThread->listItem);
		/* Add the thread to the semaphore's waiting list */
		RTOS_addThreadToSortedList(&pSemaphore->waitingList , &pRunningThread->listItem);
		/* Trigger context switch, set PendSV to pending */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
		/* Inform that we will try again from the start of this function */
		returnStatus = 2U;
	}
	else
	{
		/* Erg3 ll user ytsarf hoa :D */
	}
	return returnStatus;
}

/*
* @brief :
* @param :
* @retVal :
*/
void RTOS_semaphoreGive(RTOS_semaphore_t*pSemaphore)
{
	if(pSemaphore !=  NULL)
	{
		uint32_t terminate = 0U;
		uint32_t semaphoreTmp;
		while(1U != terminate)
		{
			semaphoreTmp = __LDREXW(&pSemaphore->semaphoreValue);
			++semaphoreTmp;
			if(0U == __STREXW(semaphoreTmp , &pSemaphore->semaphoreValue))
			{
				terminate = 1U;
				/* Required before releasing protected resource */
				__DMB();
			}
			else
			{
				/* Atomic operation was interrupted try again*/
			}
		}
		/* Check for waiting threads want the semaphore */
		if(pSemaphore->waitingList.numOfThreads > 0U)
		{
			RTOS_TCB_t *pTCB;
			/* Get the */
			pTCB = (RTOS_TCB_t*)pSemaphore->waitingList.listEnd.pNext->pContainingTCB;
			
			if(pTCB != NULL)
			{
				RTOS_removeThreadFromList(&pTCB->listItem);
				
				RTOS_addThreadToReadyList(&pTCB->listItem);
			}
			else
			{
				/* Null pointer was detected */
			}
		}
	}
	else
	{
		/* Null pointer was detected */
	}
	
}















































