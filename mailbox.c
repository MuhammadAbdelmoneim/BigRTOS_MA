

#include "mailbox.h"
#include "../RTOS_lib/memcpy.h"
#include "thread.h"


/*
* @brief  : This Function Used To Create the mailbox it runs
* @param  :
* @retVal :
*/

static void checkWaitingThreads(RTOS_mailbox_t *pMailbox)
{
	if(pMailbox != NULL)
	{

		/* If there is any threads waiting for that mailbox*/
		if(pMailbox->waitingList.numOfThreads > 0U)
		{
			RTOS_TCB_t* pThread;
			pThread = (RTOS_TCB_t*)pMailbox->waitingList.listEnd.pNext->pContainingTCB;

			RTOS_removeThreadFromList(&pThread->listItem);

			RTOS_addThreadToReadyList(&pThread->listItem);
		}
		else
		{
			/* No threads are wiating*/
		}
	}
	else
	{
		/* Inavalid parameters */
	}
}



void RTOS_mailboxCreate(RTOS_mailbox_t *pMailbox , void *pBuffer,
						uint32_t bufferSize , uint32_t messegeLength)
{
	/* Validate parameters */
	if(pMailbox != NULL && pBuffer != NULL && bufferSize != 0U && messegeLength != 0U)
	{
		pMailbox->pStrat = (uint8_t*)pBuffer;
		/* pEnd will point to the end of the buffer + 1 */
		pMailbox->pEnd = (uint8_t*)pBuffer + (bufferSize * messegeLength);
		pMailbox->bufferLength = bufferSize;
		pMailbox->messegeLength = messegeLength;
		/* Initially no messeges in the mailbox */
		pMailbox->messegeNum = 0U;

		pMailbox->pWriteIdx = pMailbox->pStrat;
		pMailbox->pReadIdx = pMailbox->pStrat;
		/* Init the mailbox waiting list */
		RTOS_initList(&pMailbox->waitingList);
	}
	else
	{
		/* Parameters Error */
	}
}

/* This Function can't be interrupt because it runs in the highest priority ISR */

uint32_t RTOS_mailboxWrite(RTOS_mailbox_t *pMailbox , uint32_t waitFlag ,
							   const void * const pMessege)
{
	uint32_t returnStatus;
	returnStatus  = 0U;
	if(pMailbox != NULL && pMessege != NULL)
	{
		if(pMailbox->bufferLength > pMailbox->messegeNum)
		{
			/* Write the data to the buffer */
			memcpyByte((uint8_t*)pMessege ,(uint8_t*)pMailbox->pWriteIdx, pMailbox->messegeLength);
			/* Increment the write index */
			pMailbox->pWriteIdx += pMailbox->messegeLength;
			/* Increment the number of messeges */
			pMailbox->messegeNum++;
			/* If the buffer if full try to come back to the start */
			if(pMailbox->pWriteIdx >= pMailbox->pEnd)
			{
				pMailbox->pWriteIdx = pMailbox->pStrat;
			}
			/* Check for waiting threads */
			checkWaitingThreads(pMailbox);
			/* Return success */
			returnStatus = 1U;
		}
		else
		{
			if(returnStatus != 1U && waitFlag == 1U)
			{
				RTOS_TCB_t *pRunningThread;
				/* Get the currentlly running thread */
				pRunningThread = RTOS_threadGetRunning();
				/* Remove this item from the ready list */
				RTOS_removeThreadFromList(&pRunningThread->listItem);
				/* Add the thread to the mailbox waiting list */
				RTOS_addThreadToSortedList(&pMailbox->waitingList,&pRunningThread->listItem);
				/* Trigger a context switch */
				SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
				/* Return Failure */
				returnStatus = 2U;
			}
			else
			{
				/* The user doesn't want to wait for the mailbox */
				returnStatus = 0U;
			}
		}
	}
	else
	{
		/* Invalid paramer*/
	}
	return returnStatus;

}






uint32_t RTOS_mailboxRead(RTOS_mailbox_t *pMailbox , uint32_t waitFlag , void *pMessege)
{
	uint32_t returnStatus;
	returnStatus = 0U;
	 if(pMailbox != NULL && pMessege != NULL && (waitFlag == 0U || waitFlag == 1U))
	 {
		 /* Check if there is messeges in the mailbox */
		 if(pMailbox->messegeNum > 0U)
		 {
			 /* Copy the data from the mailbox into the user messege variable */
			 memcpyByte((uint8_t*)pMailbox->pReadIdx,(uint8_t*)pMessege,(uint32_t)pMailbox->messegeLength);
			/* Increment the read index */
			 pMailbox->pReadIdx += pMailbox->messegeLength;
			 /* Decrement number of messegs */
			 pMailbox->messegeNum--;
			 /* Check if the read index reached the of the buffer */
			 if(pMailbox->pReadIdx >= pMailbox->pEnd)
			 {
				 /* Make the read index points back to the start of the buffer */
				 pMailbox->pReadIdx = pMailbox->pStrat;
			 }
			 /* Check for waiting threads */
			 checkWaitingThreads(pMailbox);
			 /* Return success */
			 returnStatus = 1U;
		 }
		 /* there is no messeges in the mailbox */
		 else
		 {
			 /* The user want to wait for the messege to be available */
			 if(returnStatus != 1U && waitFlag ==1U)
			 {
				 /* Get the current runnnig thread */
				 RTOS_TCB_t *pRunningThread;
				 pRunningThread = RTOS_threadGetRunning();
				 /* Remove the thread from the ready list */
				 RTOS_removeThreadFromList(&pRunningThread->listItem);
				 /* Add the thread to the mailbox waiting list */
				 RTOS_addThreadToSortedList(&pMailbox->waitingList, &pRunningThread->listItem);
				 /* Trigger a context switch */
				 SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
				 /* To try again from the start of the function when the messege available */
				 returnStatus = 2U;
			 }
			 else
			 {
				 /* Return Failure */
				 returnStatus = 0U;
			 }
		 }
	 }
	 else
	 {
		 /* Invaild parameters */
	 }
}



