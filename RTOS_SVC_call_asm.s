        AREA | .text| , CODE , READONLY , ALIGN = 2
        THUMB 
        EXPORT RTOS_SVC_schedulerStart
        EXPORT RTOS_SVC_threadCreate
        EXPORT RTOS_SVC_mutexCreate
        EXPORT RTOS_SVC_mutexLock
        EXPORT RTOS_SVC_mutexRelease
        EXPORT RTOS_SVC_semaphoreCreate
        EXPORT RTOS_SVC_semaphoreTake
        EXPORT RTOS_SVC_semaphoreGive


;This function triggers a SVC interrupt to start the scheduler
RTOS_SVC_schedulerStart
    svc 0
    bx lr


; This function triggers a SVC interrupt to create a thread
RTOS_SVC_threadCreate
    svc 1
    bx lr
    
   
; This function triggers a SVC interrupt to create mutex        
RTOS_SVC_mutexCreate
    SVC 2
    bx lr

; This function triggers a SVC interrupt to lock mutex
RTOS_SVC_mutexLock
    SVC 3
    bx lr
    
; This function triggers a SVC interrupt to release mutex 
RTOS_SVC_mutexRelease
    SVC 4
    bx lr
    

RTOS_SVC_semaphoreCreate
    SVC 5
    bx lr


RTOS_SVC_semaphoreTake
    SVC 6
    bx lr
    
RTOS_SVC_semaphoreGive
    SVC 7
    bx lr
    
    ALIGN
    END     ;end of file 