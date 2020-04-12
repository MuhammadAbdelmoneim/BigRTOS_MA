/*
 * mailbox.h
 *
 *  Created on: Mar 24, 2020
 *      Author: Muhammad_menem
 */

#ifndef MAILBOX_H_
#define MAILBOX_H_


#include <stdint.h>
#include "rtos_list.h"

/* MailBox Definition */
typedef struct RTOS_mailbox{
	uint8_t *pStrat;			/* Start of the buffer */
	uint8_t *pEnd;				/* End of the buffer */
	uint8_t *pReadIdx;			/* The position we read from */
	uint8_t *pWriteIdx;			/* The posistion we write into */
	uint32_t bufferLength;		/* Mail Box Buffer size */
	uint32_t messegeLength;		/* The messege size */
	uint32_t messegeNum; 		/* Number of Available messeges in the buffer   */
	RTOS_List_t waitingList;    /* A list of threads wants to aquire the buffer */
}RTOS_mailbox_t;

/*
* @brief :
* @param :
* @retVal :
*/

void RTOS_mailboxCreate(RTOS_mailbox_t *pMailbox , void *pBuffer,
						uint32_t bufferSize , uint32_t messegeLength);

void RTOS_SVC_mailboxCreate(RTOS_mailbox_t *pMailbox , void *pBuffer,
						uint32_t bufferSize , uint32_t messegeLength);

/*
* @brief :
* @param :
* @retVal :
*/

/*
* @brief :
* @param :
* @retVal :
*/
uint32_t RTOS_mailboxWrite(RTOS_mailbox_t *pMailbox , uint32_t waitFlag , const void * const pMessege);

uint32_t RTOS_SVC_mailboxWrite(RTOS_mailbox_t *pMailbox , uint32_t waitFlag , const void * const pMessege);
/*
* @brief :
* @param :
* @retVal :
*/



/*
* @brief :
* @param :
* @retVal :
*/


/*
* @brief :
* @param :
* @retVal :
*/

uint32_t RTOS_mailboxRead(RTOS_mailbox_t *pMailbox , uint32_t waitFlag , void *pMessege);

uint32_t RTOS_SVC_mailboxRead(RTOS_mailbox_t *pMailbox , uint32_t waitFlag , void *pMessege);

#endif /* MAILBOX_H_ */
