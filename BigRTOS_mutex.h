#ifndef BIGRTOS_MUTEX_H_
#define BIGRTOS_MUTEX_H_
#include <stdint.h>
#include "BigRTOS_list.h"
#include "stm32f407xx.h"
#include "BigRTOS_thread.h"
#include "BigRTOS_list.h"

/* Mutex Type */
typedef struct RTOS_mutex{
	/* Mutex value (0 or 1) 1 available 0 not available */
	uint8_t mutexValue;
	/* List of all threads waiting for that mutex to be released */
	RTOS_List_t pWaitingList;
}RTOS_mutex_t;



/*
* @brief :
* @param :
* @retVal :
*/
void RTOS_mutexCreate(RTOS_mutex_t *pMutex , uint32_t mutexValue);

/*
* @brief :
* @param :
* @retVal :
*/
uint32_t RTOS_mutexLock(RTOS_mutex_t *pMutex , uint32_t waitingFlag);


/*
* @brief :
* @param :
* @retVal :
*/
uint32_t RTOS_mutexRelease(RTOS_mutex_t *pMutex);





#endif //BIGRTOS_MUTEX_H_






