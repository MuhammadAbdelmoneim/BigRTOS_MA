        AREA | .text| , CODE , READONLY , ALIGN = 2
        THUMB
        EXTERN svcEXEReturn 
        EXTERN RTOS_SVC_Handler_main
        EXPORT SVC_Handler
        EXPORT PendSV_Handler
            


SVC_Handler
    tst lr, #4                ; Test bit 2 of the link register to find current stack 
    ite eq                    ; eq when zero flag is set 
    mrseq r0, msp             ; msp when bit 2 is cleared 
    mrsne r0, psp             ; psp when bit 2 is set 
    ldr r1,=svcEXEReturn      ; Load the address of svcEXEReturn in r1 
    str lr,[r1]               ; Store lr in svcEXEReturn 
    bl RTOS_SVC_Handler_main  ; Run C part of SVC Handler 
    ldr r1,=svcEXEReturn      ; Load the address of svcEXEReturn in r1 
    ldr lr,[r1]               ; Load lr with updated svcEXEReturn value
    bx lr                     ; Return 

PendSV_Handler

    bx lr                        ; Return 
    
    
    ALIGN
    END