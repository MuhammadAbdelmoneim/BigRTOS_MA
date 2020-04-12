


/* Includes */
#include  "timer_support.h"
#include "rtos_exceptions.h"
#include "../RTOS_lib/itm.h"
/* Private macro */
#define MIN_WAIT_TIME					0x00000000UL
/* Private variables */

/* Timer list used to hold the threads wants to wait for a timeout */
static RTOS_timer_list_t timerList;
/* Variable used to hold the minimum wait time */
static uint32_t minWaitTime = MIN_WAIT_TIME;
/**/

/* Private function prototypes */

/* Private functions */


void RTOS_initTimerList(void)
{
	/* Initially no threads in timer list */
	timerList.numOfThreads = 0U;
	/* Initially pIndex points to the dummyItem */
	timerList.pIndex = (RTOS_timerList_item_t*)&timerList.dummyItem;
	/* Initially the time out */
	timerList.dummyItem.waitTime = MIN_WAIT_TIME;

	timerList.dummyItem.pNext = (RTOS_timerList_item_t*)&timerList.dummyItem;;

	timerList.dummyItem.pPrev = (RTOS_timerList_item_t*)&timerList.dummyItem;;

}


void RTOS_addThreadToTimerList(RTOS_timerList_item_t *pItem)
{
	/* Validate parameters */
	ASSERT(pItem != NULL);

	uint32_t totalTime;
	totalTime = 0U;
	if(0U == timerList.numOfThreads)
	{
		timerList.dummyItem.pNext = pItem;
		timerList.dummyItem.pPrev = pItem;
		pItem->pNext = NULL;
		pItem->pPrev = NULL;
		minWaitTime = pItem->waitTime;
	}
	else
	{
		RTOS_timerList_item_t *pCurrent;
		/* Points to the first item in the list */
		pCurrent = timerList.dummyItem.pNext;
		if(pItem->waitTime <= minWaitTime)
		{
			/* Insert front */
			pItem->pNext = pCurrent;
			pItem->pPrev = NULL;
			pCurrent->pPrev = pItem;
			timerList.dummyItem.pNext = pItem;
			minWaitTime = pItem->waitTime;
			/* Update timer list items with the new time */
			uint32_t item;
			RTOS_timerList_item_t *pUpdate;
			pUpdate = timerList.dummyItem.pNext->pNext;
			pUpdate->waitTime = pUpdate->waitTime -minWaitTime;
		}
		/* pItem->waitTime >  minWaitTime*/
		else
		{
			totalTime = minWaitTime;
			while((pItem->waitTime > totalTime) && (pCurrent->pNext != NULL))
			{
				pCurrent =	pCurrent->pNext;
				totalTime += pCurrent->waitTime;
			}
			if((pCurrent->pNext == NULL) || (timerList.numOfThreads == 1))
			{
				pItem->pNext = pCurrent->pNext;
				pItem->pPrev = pCurrent;
				pCurrent->pNext = pItem;
				pItem->waitTime = pItem->waitTime - totalTime;
			}
			else if(pItem->waitTime < totalTime)
			{
				pItem->pNext = pCurrent;
				pItem->pPrev = pCurrent->pPrev;
				pCurrent->pPrev->pNext = pItem;
				pCurrent->pPrev = pItem;
				pItem->waitTime = pItem->waitTime - (totalTime - pCurrent->waitTime);
				/* Update times after the one we added */
				pCurrent->waitTime = pCurrent->waitTime - pItem->waitTime;
			}
			else
			{

			}
		}
	}
	timerList.numOfThreads++;
}

void TEST_1(void)
{
	RTOS_timerList_item_t item_1;
	RTOS_timerList_item_t item_2;
	RTOS_timerList_item_t item_3;
	RTOS_timerList_item_t item_4;
	RTOS_timerList_item_t item_5;
	RTOS_timerList_item_t item_6;

	item_1.waitTime = 10;
	item_2.waitTime = 30;
	item_3.waitTime = 70;
	item_4.waitTime = 100;
	item_5.waitTime = 4;
	item_6.waitTime = 50;

	RTOS_initTimerList();

	RTOS_addThreadToTimerList(&item_1);
	RTOS_addThreadToTimerList(&item_2);
	RTOS_addThreadToTimerList(&item_3);
	RTOS_addThreadToTimerList(&item_4);
	RTOS_addThreadToTimerList(&item_5);
	RTOS_addThreadToTimerList(&item_6);

	int i = 0;
	volatile uint32_t val;
	RTOS_timerList_item_t *pCurrent;
	pCurrent = timerList.dummyItem.pNext;
	for( i = 0; i < 6; i++)
	{
		val = pCurrent->waitTime;
		pCurrent = pCurrent->pNext;
	}
	val++;

}

void RTOS_removeThreadFromTimerList(void)
{

}


void RTOS_threadSleep(uint32_t waitTime)
{
	/* Add the running thread to the timer list */
	/* Get the running thread */

}
