/* Files includes */
#include "BigRTOS_list.h"
#include "c_config.h"

/**
 * @brief   List end item value
 */
#define LIST_END_ITEM_VALUE                     ((uint32_t) 0xFFFFFFFFu)

void RTOS_initList(RTOS_List_t *pList)
{
	pList->numOfThreads = 0U;
	
	pList->pIndex = (RTOS_list_item_t*)&pList->listEnd;
	
	pList->listEnd.pNext = (RTOS_list_item_t*)&pList->listEnd;
	
	pList->listEnd.pPrev = (RTOS_list_item_t*)&pList->listEnd;
	
	pList->listEnd.itemValue = LIST_END_ITEM_VALUE;
}



/* Adds a thread to the end of the list */
void RTOS_addThreadToList(RTOS_List_t *pList , RTOS_list_item_t *pNewItem)
{ 
	if(pList == NULL || pNewItem == NULL)
	{
		/* NULL pointer Exception */
	}
	else
	{
		pNewItem->pNext = pList->pIndex->pNext;
	
		pNewItem->pPrev = pList->pIndex;
		
		pList->pIndex->pNext->pPrev = pNewItem;
		
		pList->pIndex->pNext = pNewItem;
		
		pList->pIndex = pNewItem;
		
		pNewItem->pContainingList = (RTOS_List_t*)pList;
		
		pList->numOfThreads++;
	}
	
}


void RTOS_addThreadToSortedList(RTOS_List_t* pList , RTOS_list_item_t* pNewItem)
{
		/* Temp for the insert index */
	  RTOS_list_item_t * pInsertIndex = (RTOS_list_item_t *) &pList->listEnd;

	  /* Get insert index, find the high item value */
	  while((pInsertIndex->pNext != (RTOS_list_item_t *) &pList->listEnd)
		  && (pInsertIndex->pNext->itemValue <= pNewItem->itemValue))
	  {
		pInsertIndex = pInsertIndex->pNext;
	  }

	  /* Connect the new item with insert index */
	  pNewItem->pNext = pInsertIndex->pNext;
	  pNewItem->pPrev = pInsertIndex;
	  pInsertIndex->pNext->pPrev = pNewItem;
	  pInsertIndex->pNext = pNewItem;

	  /* Set the list container for the new item */
	  pNewItem->pContainingList = (void *) pList;

	  /* Increment number of items in the list */
	  pList->numOfThreads++;
}

void RTOS_removeThreadFromList(RTOS_list_item_t*pRemovedItem)
	{
		 /* Previous of the next item will be the previous of the removed item */
	  pRemovedItem->pNext->pPrev = pRemovedItem->pPrev;

	  /* Next of the previous item will the next of the removed item */
	  pRemovedItem->pPrev->pNext = pRemovedItem->pNext;

	  /* Temp list pointer */
	  RTOS_List_t * pList;

	  /* Get list of the removed item */
	  pList = (RTOS_List_t *) pRemovedItem->pContainingList;

	  /* If the index pointing to the removed item,
	   * make it points the previous item */
	  if(pRemovedItem == pList->pIndex)
	  {
		pList->pIndex = pRemovedItem->pPrev;
	  }
	  else
	  {
		/* Index pointing somewhere else */
	  }

	  /* Remove item from the list */
	  pRemovedItem->pContainingList = NULL;

	  /* Decrement number of thread items in the list */
	  pList->numOfThreads--;
	
}





















