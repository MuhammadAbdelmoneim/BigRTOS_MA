#include "stm32f407xx.h"
#include "RTOS.h"
#include "itm.h"





#define GPIOD_BASE_ADD 			((uint32_t)0x40020C00)
#define GPIOD_MODE_R_OFFSET		((uint32_t)(0x00))
#define GPIOD_DATA_R_OFFSET		((uint32_t)(0x14))
#define GPIOD_SET_R_OFFSET		((uint32_t)(0x18))

#define GPIOD_MODE_R			(*((volatile uint32_t*)(GPIOD_BASE_ADD + GPIOD_MODE_R_OFFSET)))	
#define GPIOD_DATA_OUT_R		(*((volatile uint32_t*)(GPIOD_BASE_ADD + GPIOD_DATA_R_OFFSET)))
#define GPIOD_SET 				(*((volatile uint32_t*)(GPIOD_BASE_ADD + GPIOD_SET_R_OFFSET)))

uint32_t counter = 0;

RTOS_mutex_t m1;
RTOS_semaphore_t sm;

void LED(void)
{
	
	
		/*RCC->AHB1ENR |= (1<<3);
		GPIOD_MODE_R |= (1 << (15 * 2));
		GPIOD_DATA_OUT_R |= (1<< (15));*/
	while(1)
	{
		RTOS_SVC_semaphoreTake(&sm,1);
		//RTOS_SVC_mutexLock(&m1,1);
		ITM_Printf("Hello Muhammad thread1\n");
		ITM_Printf("******************\n");
		counter++;
		RTOS_SVC_mutexRelease(&m1);
		RTOS_SVC_semaphoreGive(&sm);
	}
}

RTOS_TCB_t t1;
RTOS_stack_t s1;

void LED2(void)
{
	
		/*RCC->AHB1ENR |= (1<<3);
		GPIOD_MODE_R |= (1 << (13 * 2));
		GPIOD_DATA_OUT_R |= (1<< (13));*/
	while(1)
	{
		RTOS_SVC_semaphoreTake(&sm,1);
		//RTOS_SVC_mutexLock(&m1,1);
		ITM_Printf("Hello Muhammad thread2\n");
		ITM_Printf("******************\n");
		counter--;
		//RTOS_SVC_mutexRelease(&m1);
		RTOS_SVC_semaphoreGive(&sm);
	}
}

RTOS_TCB_t t2;
RTOS_stack_t s2;



void LED3(void)
{
	
		/*RCC->AHB1ENR |= (1<<3);
		GPIOD_MODE_R |= (1 << (12 * 2));
		GPIOD_DATA_OUT_R |= (1<< (12));*/
	while(1)
	{
		RTOS_SVC_semaphoreTake(&sm,1);
		//RTOS_SVC_mutexLock(&m1,1);
		ITM_Printf("Hello Muhammad thread3\n");
		ITM_Printf("******************\n");
		counter++;
		//RTOS_SVC_mutexRelease(&m1);
		RTOS_SVC_semaphoreGive(&sm);

	}
}


RTOS_TCB_t t3;
RTOS_stack_t s3;

void LED4(void)
{
	
		/*RCC->AHB1ENR |= (1<<3);
		GPIOD_MODE_R |= (1 << (14 * 2));
		GPIOD_DATA_OUT_R |= (1<< (14));*/
	while(1)
	{
		RTOS_SVC_semaphoreTake(&sm,1);
		//RTOS_SVC_mutexLock(&m1,1);
		ITM_Printf("Hello Muhammad thread4\n");
		ITM_Printf("******************\n");
		counter--;
		//RTOS_SVC_mutexRelease(&m1);
		RTOS_SVC_semaphoreGive(&sm);
	}
}


RTOS_TCB_t t4;
RTOS_stack_t s4;




int main(void)
{
	RTOS_init();
	RTOS_SVC_threadCreate(&t1,&s1,0,LED);
	RTOS_SVC_threadCreate(&t2,&s2,0,LED2);
	RTOS_SVC_threadCreate(&t3,&s3,0,LED3);
	RTOS_SVC_threadCreate(&t4,&s4,0,LED4);
	//RTOS_SVC_mutexCreate(&m1,1);
	RTOS_SVC_semaphoreCreate(&sm,1);
	RTOS_SVC_schedulerStart();
	
	while(1)
	{
		
	}
}

	


