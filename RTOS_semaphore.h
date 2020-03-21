#ifndef RTOS_SEMAPHORE_H_
#define RTOS_SEMAPHORE_H_
#include <stdint.h>
#include "BigRTOS_list.h"


typedef struct RTOS_semaphre{
	uint32_t semaphoreValue;
	RTOS_List_t waitingList; 

}RTOS_semaphore_t;


/*
* @brief :
* @param :
* @retVal :
*/
void RTOS_semaphoreCreate(RTOS_semaphore_t *pSemaphore , uint32_t semaphoreValue);

/*
* @brief :
* @param :
* @retVal :
*/
uint32_t RTOS_semaphoreTake(RTOS_semaphore_t*pSemaphore , uint32_t waitingFlag);

/*
* @brief :
* @param :
* @retVal :
*/
void RTOS_semaphoreGive(RTOS_semaphore_t*pSemaphore);




#endif //RTOS_SEMAPHORE_H_
