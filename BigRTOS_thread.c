#include "BigRTOS_thread.h"

/* Number of threads */
static uint32_t numOfThreads = 0U;
/* The curently running thread */
static RTOS_TCB_t *pRunningThread = NULL;
/* Running thread ID*/
static uint32_t runningThreadID;
/* Next thread to run */
static RTOS_list_item_t *pNextThread;
/* Current Top priority*/
static uint32_t currentTopPriority = RTOS_NUM_OF_PRIORTY_LEVELS - 1;
/* Ready List */
static RTOS_List_t readyList[RTOS_NUM_OF_PRIORTY_LEVELS];



void RTOS_threadInitList(void)
{
	uint32_t priority;
	for(priority = 0; priority < RTOS_NUM_OF_PRIORTY_LEVELS; priority++)
	{
		RTOS_initList(&readyList[priority]);
	}
	
}


void RTOS_threadCreate(RTOS_TCB_t *pTCB ,RTOS_stack_t *pThreadStack ,uint32_t priority, void *pThreadFunction)
{
	
	if((pTCB != NULL) && (pThreadStack != NULL))
	{
		/* The start address of thr thread stack when we do context switch */
		pTCB->pStackPointer = ((uint32_t)pThreadStack->stack  + (RTOS_THREAD_STACK_SIZE * 8)) - (18 * 4);
		/* Write thread function start address so when run it we start from this address */
		MEM32_ADDRESS((pTCB->pStackPointer + (16 << 2))) = (uint32_t)pThreadFunction;
		/* Write xPSR indicates , THUMB ISA */
		MEM32_ADDRESS((pTCB->pStackPointer + (17 << 2))) = 0x01000000;
		/* Write EXC_RETURN, since the execution threads are using PSP, this will
		 * allow SVC to return to the thread with PSP */
		MEM32_ADDRESS(pTCB->pStackPointer) = 0xFFFFFFFDUL;
		/* Write initial CONTROL register value UNPRIVILEGED, PSP & no FPU */
		MEM32_ADDRESS((pTCB->pStackPointer + (1 << 2))) = 0x3;
		/* Link the TCB with the list item */
		pTCB->listItem.pContainingTCB = (RTOS_TCB_t *)pTCB;
		/* Assign priorty */
		pTCB->threadPriority = priority;
		/* Assign thread function */
		pTCB->pThreadFun = pThreadFunction;
		/* Increment number of threads in the system */
		pTCB->threadID = ++numOfThreads;
		/* Add the created thread to the ready list */
		RTOS_addThreadToList(&readyList[pTCB->threadPriority],&pTCB->listItem);
		/* Item Value used to sort the list in priority order */
		pTCB->listItem.itemValue = priority;
		/* Set current top priority */
	    if(pTCB->threadPriority < currentTopPriority)
	    {
		  currentTopPriority = pTCB->threadPriority;
	    }

	    /* Check the need for context switch when scheduler is running
	     * and this thread is the higher priority than the running thread */
	    if((NULL != pRunningThread)
		    && (pTCB->threadPriority < pRunningThread->threadPriority))
	    {
		  /* Trigger context switch, set PendSV to pending */
		  SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	    }
	    else
	    {
		  /* Context switch is not required */
	    }
	}
	else
	{
		/* NULL PTR */
	}
}

void RTOS_threadSwitchRunning(void)
{	
	while(readyList[currentTopPriority].numOfThreads == 0U)
	{
		currentTopPriority++;
	}
	RTOS_List_t*pList = &readyList[currentTopPriority];
	/* Get the thread to run */
	pList->pIndex = pList->pIndex->pNext;
	if(pList->pIndex == (RTOS_list_item_t*)&readyList[currentTopPriority].listEnd)
	{
		pList->pIndex = readyList[currentTopPriority].pIndex->pNext;
	}
	pRunningThread = (RTOS_TCB_t*)pList->pIndex->pContainingTCB;
	
	runningThreadID = pRunningThread->threadID;
	
}

RTOS_TCB_t *RTOS_threadGetRunning(void)
{
	return pRunningThread;
}


void RTOS_addThreadToReadyList(RTOS_list_item_t *pNewItem)
{
	RTOS_TCB_t* pTCB = (RTOS_TCB_t*)pNewItem->pContainingTCB;
	
	RTOS_addThreadToList(&readyList[pTCB->threadPriority],pNewItem);
	/* Set current top priority */
	if(pTCB->threadPriority < currentTopPriority)
	{
	  currentTopPriority = pTCB->threadPriority;
	}

	/* Check the need for context switch when scheduler is running
	 * and this thread is the higher priority than the running thread */
	if((NULL != pRunningThread)
		&& (pTCB->threadPriority < pRunningThread->threadPriority))
	{
	  /* Trigger context switch, set PendSV to pending */
	  SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	else
	{
	  /* Context switch is not required */
	}
}
