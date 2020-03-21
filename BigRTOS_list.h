#ifndef BIGRTOS_LIST_H_
#define BIGRTOS_LIST_H_

#include <stdint.h>
#include "c_config.h"




typedef struct RTOS_list_item{
	uint32_t itemValue;
	struct RTOS_list_item *pNext;
	struct RTOS_list_item *pPrev;
	/* Pointer to the TCB containing this item  this pointer will be casted to TCB type */
	void *pContainingTCB;
	void *pContainingList;
}RTOS_list_item_t;


typedef struct RTOS_list_end{
	uint32_t itemValue;
	RTOS_list_item_t *pNext;
	RTOS_list_item_t *pPrev;
}RTOS_list_end_t;



typedef struct RTOS_TCB_List{
	uint32_t  numOfThreads;
	/* points to the thread to be run when we do context switch */
	RTOS_list_item_t *pIndex;
	RTOS_list_end_t listEnd;
}RTOS_List_t;



void RTOS_initList(RTOS_List_t *pList);

void RTOS_addThreadToList(RTOS_List_t *pList , RTOS_list_item_t *pNewItem);



void RTOS_addThreadToSortedList(RTOS_List_t* pList , RTOS_list_item_t* pNewItem);

void RTOS_removeThreadFromList(RTOS_list_item_t*pRemovedItem);


#endif //BIGRTOS_LIST_H_

