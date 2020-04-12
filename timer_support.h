/*
 * timer_support.h
 *
 *  Created on: Mar 26, 2020
 *      Author: Muhammad_menem
 */

#ifndef TIMER_SUPPORT_H_
#define TIMER_SUPPORT_H_

/* Includes */
#include "thread.h"
#include <stdint.h>

/* Public types */
typedef struct RTOS_timerList_item{
	/* Used in sorting the list */
	uint32_t itemValue;
	/* The blocking time for this thread */
	uint32_t waitTime;
	/* Points to the next item */
	struct RTOS_timerList_item *pNext;
	/* points to the previous item */
	struct RTOS_timerList_item *pPrev;
	/* Pointer to the TCB containing this item*/
	void *pContainingTCB;
	/* Pointer to the event list (mutex ,semaphore .....) */
	void *pEventList;
	/* Pointer to the containing list */
	void *pContainingList;
}RTOS_timerList_item_t;


/* Linked list head item (the only item when the list is empty) */
typedef struct RTOS_timer_list_end{
	/* Blocking time */
	uint32_t waitTime;
	/* Used in sorting the list */
	uint32_t itemValue;
	/* Points to the first item in the list */
	RTOS_timerList_item_t *pNext;
	/* Points to the last item in the list */
	RTOS_timerList_item_t *pPrev;
}RTOS_timer_list_end_t;


/* Threads list type (list of threads in the timer list) */
typedef struct RTOS_timer_list{
	uint32_t  numOfThreads;
	/* points to  the lowest timeout thread */
	RTOS_timerList_item_t *pIndex;
	/* Dummy head */
	RTOS_timer_list_end_t dummyItem;
}RTOS_timer_list_t;

/**
  * @brief : Function used to initialize the timer list
  * @param  None
  * @retval None
  */
void RTOS_initTimerList(void);

/**
  * @brief : Function used to add a thread to the timer list
  * @param  pointer to the thread's list item
  * @retval None
  */
void RTOS_addThreadToTimerList(RTOS_timerList_item_t *pItem);

/**
  * @brief : Function used to remove a thread from the timer list
  * @param  None
  * @retval None
  */
void RTOS_removeThreadFromTimerList(void);

/**
  * @brief : Function used to make the current Running thread to sleep(delay) for waitTime
  * @param  The sleep(delay) time
  * @retval None
  */
void RTOS_threadSleep(uint32_t waitTime);
void TEST_1(void);

#endif /* TIMER_SUPPORT_H_ */
