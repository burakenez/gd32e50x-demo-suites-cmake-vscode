  .syntax unified
  .cpu cortex-m33
  .fpu softvfp
  .thumb

.global  Default_Handler

/* necessary symbols defined in linker script to initialize data */
.word  _sidata
.word  _sdata
.word  _edata
.word  _sbss
.word  _ebss

.equ  BootRAM,        0xF1E0F85F

  .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function

/* reset Handler */
Reset_Handler:
  movs r1, #0
  b DataInit

CopyData:
  ldr r3, =_sidata
  ldr r3, [r3, r1]
  str r3, [r0, r1]
  adds r1, r1, #4

DataInit:
  ldr r0, =_sdata
  ldr r3, =_edata
  adds r2, r0, r1
  cmp r2, r3
  bcc CopyData
  ldr r2, =_sbss
  b Zerobss

FillZerobss:
  movs r3, #0
  str r3, [r2], #4

Zerobss:
  ldr r3, = _ebss
  cmp r2, r3
  bcc FillZerobss
/* Call SystemInit function */
  bl  SystemInit
/* Call static constructors */
    bl __libc_init_array
/*Call the main function */
  bl  main

LoopForever:
    b LoopForever

.size  Reset_Handler, .-Reset_Handler

    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler

   .section  .vectors,"a",%progbits
   .global __gVectors
__gVectors:
                    .word _sp                                     /* Top of stack */
                    .word Reset_Handler                           /* Vector Number 1,Reset Handler */
                    .word NMI_Handler                             /* Vector Number 2,NMI Handler */
                    .word HardFault_Handler                       /* Vector Number 3,Hard Fault Handler */
                    .word MemManage_Handler                       /* Vector Number 4,MPU Fault Handler */
                    .word BusFault_Handler                        /* Vector Number 5,Bus Fault Handler */
                    .word UsageFault_Handler                      /* Vector Number 6,Usage Fault Handler */
                    .word 0                                       /* Reserved */
                    .word 0                                       /* Reserved */
                    .word 0                                       /* Reserved */
                    .word 0                                       /* Reserved */
                    .word SVC_Handler                             /* Vector Number 11,SVCall Handler */
                    .word DebugMon_Handler                        /* Vector Number 12,Debug Monitor Handler */
                    .word 0                                       /* Reserved */
                    .word PendSV_Handler                          /* Vector Number 14,PendSV Handler */
                    .word SysTick_Handler                         /* Vector Number 15,SysTick Handler */

                    /* External interrupts handler */
                    .word WWDGT_IRQHandler                        /* Vector Number 16,Window Watchdog Timer */
                    .word LVD_IRQHandler                          /* Vector Number 17,LVD through EXTI Line detect */
                    .word TAMPER_IRQHandler                       /* Vector Number 18,Tamper through EXTI Line detect */
                    .word RTC_IRQHandler                          /* Vector Number 19,RTC through EXTI Line */
                    .word FMC_IRQHandler                          /* Vector Number 20,FMC */
                    .word RCU_CTC_IRQHandler                      /* Vector Number 21,RCU and CTC */
                    .word EXTI0_IRQHandler                        /* Vector Number 22,EXTI Line 0 */
                    .word EXTI1_IRQHandler                        /* Vector Number 23,EXTI Line 1 */
                    .word EXTI2_IRQHandler                        /* Vector Number 24,EXTI Line 2 */
                    .word EXTI3_IRQHandler                        /* Vector Number 25,EXTI Line 3 */
                    .word EXTI4_IRQHandler                        /* Vector Number 26,EXTI Line 4 */
                    .word DMA0_Channel0_IRQHandler                /* Vector Number 27,DMA0 Channel0 */
                    .word DMA0_Channel1_IRQHandler                /* Vector Number 28,DMA0 Channel1 */
                    .word DMA0_Channel2_IRQHandler                /* Vector Number 29,DMA0 Channel2 */
                    .word DMA0_Channel3_IRQHandler                /* Vector Number 30,DMA0 Channel3 */
                    .word DMA0_Channel4_IRQHandler                /* Vector Number 31,DMA0 Channel4 */
                    .word DMA0_Channel5_IRQHandler                /* Vector Number 32,DMA0 Channel5 */
                    .word DMA0_Channel6_IRQHandler                /* Vector Number 33,DMA0 Channel6 */
                    .word ADC0_1_IRQHandler                       /* Vector Number 34,ADC0 and ADC1 */
                    .word CAN0_TX_IRQHandler                      /* Vector Number 35,CAN0 TX */
                    .word CAN0_RX0_IRQHandler                     /* Vector Number 36,CAN0 RX0 */
                    .word CAN0_RX1_IRQHandler                     /* Vector Number 37,CAN0 RX1 */
                    .word CAN0_EWMC_IRQHandler                    /* Vector Number 38,CAN0 EWMC */
                    .word EXTI5_9_IRQHandler                      /* Vector Number 39,EXTI5 to EXTI9 */
                    .word TIMER0_BRK_TIMER8_IRQHandler            /* Vector Number 40,TIMER0 Break and TIMER8 */
                    .word TIMER0_UP_TIMER9_IRQHandler             /* Vector Number 41,TIMER0 Update and TIMER9 */
                    .word TIMER0_TRG_CMT_TIMER10_IRQHandler       /* Vector Number 42,TIMER0 Trigger and Commutation and TIMER10 */
                    .word TIMER0_Channel_IRQHandler               /* Vector Number 43,TIMER0 Channel Capture Compare */
                    .word TIMER1_IRQHandler                       /* Vector Number 44,TIMER1 */
                    .word TIMER2_IRQHandler                       /* Vector Number 45,TIMER2 */
                    .word TIMER3_IRQHandler                       /* Vector Number 46,TIMER3 */
                    .word I2C0_EV_IRQHandler                      /* Vector Number 47,I2C0 Event */
                    .word I2C0_ER_IRQHandler                      /* Vector Number 48,I2C0 Error */
                    .word I2C1_EV_IRQHandler                      /* Vector Number 49,I2C1 Event */
                    .word I2C1_ER_IRQHandler                      /* Vector Number 50,I2C1 Error */
                    .word SPI0_IRQHandler                         /* Vector Number 51,SPI0 */
                    .word SPI1_I2S1ADD_IRQHandler                 /* Vector Number 52,SPI1 or I2S1ADD */
                    .word USART0_IRQHandler                       /* Vector Number 53,USART0 */
                    .word USART1_IRQHandler                       /* Vector Number 54,USART1 */
                    .word USART2_IRQHandler                       /* Vector Number 55,USART2 */
                    .word EXTI10_15_IRQHandler                    /* Vector Number 56,EXTI10 to EXTI15 */
                    .word RTC_Alarm_IRQHandler                    /* Vector Number 57,RTC Alarm */
                    .word USBHS_WKUP_IRQHandler                   /* Vector Number 58,USBHS Wakeup */
                    .word TIMER7_BRK_TIMER11_IRQHandler           /* Vector Number 59,TIMER7 Break and TIMER11 */
                    .word TIMER7_UP_TIMER12_IRQHandler            /* Vector Number 60,TIMER7 Update and TIMER12 */
                    .word TIMER7_TRG_CMT_TIMER13_IRQHandler       /* Vector Number 61,TIMER7 Trigger and Commutation and TIMER13 */
                    .word TIMER7_Channel_IRQHandler               /* Vector Number 62,TIMER7 Channel Capture Compare */
                    .word 0                                       /* Reserved */
                    .word EXMC_IRQHandler                         /* Vector Number 64,EXMC */
                    .word 0                                       /* Reserved */
                    .word TIMER4_IRQHandler                       /* Vector Number 66,TIMER4 */
                    .word SPI2_I2S2ADD_IRQHandler                 /* Vector Number 67,SPI2 or I2S2ADD */
                    .word UART3_IRQHandler                        /* Vector Number 68,UART3 */
                    .word UART4_IRQHandler                        /* Vector Number 69,UART4 */
                    .word TIMER5_DAC_IRQHandler                   /* Vector Number 70,TIMER5 or DAC */
                    .word TIMER6_IRQHandler                       /* Vector Number 71,TIMER6 */
                    .word DMA1_Channel0_IRQHandler                /* Vector Number 72,DMA1 Channel0 */
                    .word DMA1_Channel1_IRQHandler                /* Vector Number 73,DMA1 Channel1 */
                    .word DMA1_Channel2_IRQHandler                /* Vector Number 74,DMA1 Channel2 */
                    .word DMA1_Channel3_IRQHandler                /* Vector Number 75,DMA1 Channel3 */
                    .word DMA1_Channel4_IRQHandler                /* Vector Number 76,DMA1 Channel4 */
                    .word ENET_IRQHandler                         /* Vector Number 77,Ethernet */
                    .word ENET_WKUP_IRQHandler                    /* Vector Number 78,Ethernet Wakeup through EXTI Line */
                    .word CAN1_TX_IRQHandler                      /* Vector Number 79,CAN1 TX */
                    .word CAN1_RX0_IRQHandler                     /* Vector Number 80,CAN1 RX0 */
                    .word CAN1_RX1_IRQHandler                     /* Vector Number 81,CAN1 RX1 */
                    .word CAN1_EWMC_IRQHandler                    /* Vector Number 82,CAN1 EWMC */
                    .word USBHS_IRQHandler                        /* Vector Number 83,USBHS */
                    .word 0                                       /* Reserved */
                    .word SHRTIMER_IRQ2_IRQHandler                /* Vector Number 85,SHRTIMER IRQ2 */
                    .word SHRTIMER_IRQ3_IRQHandler                /* Vector Number 86,SHRTIMER IRQ3 */
                    .word SHRTIMER_IRQ4_IRQHandler                /* Vector Number 87,SHRTIMER IRQ4 */
                    .word SHRTIMER_IRQ5_IRQHandler                /* Vector Number 88,SHRTIMER IRQ5 */
                    .word SHRTIMER_IRQ6_IRQHandler                /* Vector Number 89,SHRTIMER IRQ6 */
                    .word USBHS_EP1_OUT_IRQHandler                /* Vector Number 90,USBHS end point 1 out */
                    .word USBHS_EP1_IN_IRQHandler                 /* Vector Number 91,USBHS end point 1 in */
                    .word SHRTIMER_IRQ0_IRQHandler                /* Vector Number 92,SHRTIMER IRQ0 */
                    .word SHRTIMER_IRQ1_IRQHandler                /* Vector Number 93,SHRTIMER IRQ1 */
                    .word CAN2_TX_IRQHandler                      /* Vector Number 94,CAN2 TX */
                    .word CAN2_RX0_IRQHandler                     /* Vector Number 95,CAN2 RX0 */
                    .word CAN2_RX1_IRQHandler                     /* Vector Number 96,CAN2 RX1 */
                    .word CAN2_EWMC_IRQHandler                    /* Vector Number 97,CAN2 EWMC */
                    .word I2C2_EV_IRQHandler                      /* Vector Number 98,I2C2 Event */
                    .word I2C2_ER_IRQHandler                      /* Vector Number 99,I2C2 Error */
                    .word USART5_IRQHandler                       /* Vector Number 100,USART5 */
                    .word I2C2_WKUP_IRQHandler                    /* Vector Number 101,I2C2 Wakeup */
                    .word USART5_WKUP_IRQHandler                  /* Vector Number 102,USART5 Wakeup */
                    .word TMU_IRQHandler                          /* Vector Number 103,TMU */

  .size   __gVectors, . - __gVectors

  .weak NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak MemManage_Handler
  .thumb_set MemManage_Handler,Default_Handler

  .weak BusFault_Handler
  .thumb_set BusFault_Handler,Default_Handler

  .weak UsageFault_Handler
  .thumb_set UsageFault_Handler,Default_Handler

  .weak SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak DebugMon_Handler
  .thumb_set DebugMon_Handler,Default_Handler

  .weak PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak WWDGT_IRQHandler
  .thumb_set WWDGT_IRQHandler,Default_Handler

  .weak LVD_IRQHandler
  .thumb_set LVD_IRQHandler,Default_Handler

  .weak TAMPER_IRQHandler
  .thumb_set TAMPER_IRQHandler,Default_Handler

  .weak RTC_IRQHandler
  .thumb_set RTC_IRQHandler,Default_Handler

  .weak FMC_IRQHandler
  .thumb_set FMC_IRQHandler,Default_Handler

  .weak RCU_CTC_IRQHandler
  .thumb_set RCU_CTC_IRQHandler,Default_Handler

  .weak EXTI0_IRQHandler
  .thumb_set EXTI0_IRQHandler,Default_Handler

  .weak EXTI1_IRQHandler
  .thumb_set EXTI1_IRQHandler,Default_Handler

  .weak EXTI2_IRQHandler
  .thumb_set EXTI2_IRQHandler,Default_Handler

  .weak EXTI3_IRQHandler
  .thumb_set EXTI3_IRQHandler,Default_Handler

  .weak EXTI4_IRQHandler
  .thumb_set EXTI4_IRQHandler,Default_Handler

  .weak DMA0_Channel0_IRQHandler
  .thumb_set DMA0_Channel0_IRQHandler,Default_Handler

  .weak DMA0_Channel1_IRQHandler
  .thumb_set DMA0_Channel1_IRQHandler,Default_Handler

  .weak DMA0_Channel2_IRQHandler
  .thumb_set DMA0_Channel2_IRQHandler,Default_Handler

  .weak DMA0_Channel3_IRQHandler
  .thumb_set DMA0_Channel3_IRQHandler,Default_Handler

  .weak DMA0_Channel4_IRQHandler
  .thumb_set DMA0_Channel4_IRQHandler,Default_Handler

  .weak DMA0_Channel5_IRQHandler
  .thumb_set DMA0_Channel5_IRQHandler,Default_Handler

  .weak DMA0_Channel6_IRQHandler
  .thumb_set DMA0_Channel6_IRQHandler,Default_Handler

  .weak ADC0_1_IRQHandler
  .thumb_set ADC0_1_IRQHandler,Default_Handler

  .weak CAN0_TX_IRQHandler
  .thumb_set CAN0_TX_IRQHandler,Default_Handler

  .weak CAN0_RX0_IRQHandler
  .thumb_set CAN0_RX0_IRQHandler,Default_Handler

  .weak CAN0_RX1_IRQHandler
  .thumb_set CAN0_RX1_IRQHandler,Default_Handler

  .weak CAN0_EWMC_IRQHandler
  .thumb_set CAN0_EWMC_IRQHandler,Default_Handler

  .weak EXTI5_9_IRQHandler
  .thumb_set EXTI5_9_IRQHandler,Default_Handler

  .weak TIMER0_BRK_TIMER8_IRQHandler
  .thumb_set TIMER0_BRK_TIMER8_IRQHandler,Default_Handler

  .weak TIMER0_UP_TIMER9_IRQHandler
  .thumb_set TIMER0_UP_TIMER9_IRQHandler,Default_Handler

  .weak TIMER0_TRG_CMT_TIMER10_IRQHandler
  .thumb_set TIMER0_TRG_CMT_TIMER10_IRQHandler,Default_Handler

  .weak TIMER0_Channel_IRQHandler
  .thumb_set TIMER0_Channel_IRQHandler,Default_Handler

  .weak TIMER1_IRQHandler
  .thumb_set TIMER1_IRQHandler,Default_Handler

  .weak TIMER2_IRQHandler
  .thumb_set TIMER2_IRQHandler,Default_Handler

  .weak TIMER3_IRQHandler
  .thumb_set TIMER3_IRQHandler,Default_Handler

  .weak I2C0_EV_IRQHandler
  .thumb_set I2C0_EV_IRQHandler,Default_Handler

  .weak I2C0_ER_IRQHandler
  .thumb_set I2C0_ER_IRQHandler,Default_Handler

  .weak I2C1_EV_IRQHandler
  .thumb_set I2C1_EV_IRQHandler,Default_Handler

  .weak I2C1_ER_IRQHandler
  .thumb_set I2C1_ER_IRQHandler,Default_Handler

  .weak SPI0_IRQHandler
  .thumb_set SPI0_IRQHandler,Default_Handler

  .weak SPI1_I2S1ADD_IRQHandler
  .thumb_set SPI1_I2S1ADD_IRQHandler,Default_Handler

  .weak USART0_IRQHandler
  .thumb_set USART0_IRQHandler,Default_Handler

  .weak USART1_IRQHandler
  .thumb_set USART1_IRQHandler,Default_Handler

  .weak USART2_IRQHandler
  .thumb_set USART2_IRQHandler,Default_Handler

  .weak EXTI10_15_IRQHandler
  .thumb_set EXTI10_15_IRQHandler,Default_Handler

  .weak RTC_Alarm_IRQHandler
  .thumb_set RTC_Alarm_IRQHandler,Default_Handler

  .weak USBHS_WKUP_IRQHandler
  .thumb_set USBHS_WKUP_IRQHandler,Default_Handler

  .weak TIMER7_BRK_TIMER11_IRQHandler
  .thumb_set TIMER7_BRK_TIMER11_IRQHandler,Default_Handler

  .weak TIMER7_UP_TIMER12_IRQHandler
  .thumb_set TIMER7_UP_TIMER12_IRQHandler,Default_Handler

  .weak TIMER7_TRG_CMT_TIMER13_IRQHandler
  .thumb_set TIMER7_TRG_CMT_TIMER13_IRQHandler,Default_Handler

  .weak TIMER7_Channel_IRQHandler
  .thumb_set TIMER7_Channel_IRQHandler,Default_Handler

  .weak EXMC_IRQHandler
  .thumb_set EXMC_IRQHandler,Default_Handler

  .weak TIMER4_IRQHandler
  .thumb_set TIMER4_IRQHandler,Default_Handler

  .weak SPI2_I2S2ADD_IRQHandler
  .thumb_set SPI2_I2S2ADD_IRQHandler,Default_Handler

  .weak UART3_IRQHandler
  .thumb_set UART3_IRQHandler,Default_Handler

  .weak UART4_IRQHandler
  .thumb_set UART4_IRQHandler,Default_Handler

  .weak TIMER5_DAC_IRQHandler
  .thumb_set TIMER5_DAC_IRQHandler,Default_Handler

  .weak TIMER6_IRQHandler
  .thumb_set TIMER6_IRQHandler,Default_Handler

  .weak DMA1_Channel0_IRQHandler
  .thumb_set DMA1_Channel0_IRQHandler,Default_Handler

  .weak DMA1_Channel1_IRQHandler
  .thumb_set DMA1_Channel1_IRQHandler,Default_Handler

  .weak DMA1_Channel2_IRQHandler
  .thumb_set DMA1_Channel2_IRQHandler,Default_Handler

  .weak DMA1_Channel3_IRQHandler
  .thumb_set DMA1_Channel3_IRQHandler,Default_Handler

  .weak DMA1_Channel4_IRQHandler
  .thumb_set DMA1_Channel4_IRQHandler,Default_Handler

  .weak ENET_IRQHandler
  .thumb_set ENET_IRQHandler,Default_Handler

  .weak ENET_WKUP_IRQHandler
  .thumb_set ENET_WKUP_IRQHandler,Default_Handler

  .weak CAN1_TX_IRQHandler
  .thumb_set CAN1_TX_IRQHandler,Default_Handler

  .weak CAN1_RX0_IRQHandler
  .thumb_set CAN1_RX0_IRQHandler,Default_Handler

  .weak CAN1_RX1_IRQHandler
  .thumb_set CAN1_RX1_IRQHandler,Default_Handler

  .weak CAN1_EWMC_IRQHandler
  .thumb_set CAN1_EWMC_IRQHandler,Default_Handler

  .weak USBHS_IRQHandler
  .thumb_set USBHS_IRQHandler,Default_Handler

  .weak SHRTIMER_IRQ2_IRQHandler
  .thumb_set SHRTIMER_IRQ2_IRQHandler,Default_Handler

  .weak SHRTIMER_IRQ3_IRQHandler
  .thumb_set SHRTIMER_IRQ3_IRQHandler,Default_Handler

  .weak SHRTIMER_IRQ4_IRQHandler
  .thumb_set SHRTIMER_IRQ4_IRQHandler,Default_Handler

  .weak SHRTIMER_IRQ5_IRQHandler
  .thumb_set SHRTIMER_IRQ5_IRQHandler,Default_Handler

  .weak SHRTIMER_IRQ6_IRQHandler
  .thumb_set SHRTIMER_IRQ6_IRQHandler,Default_Handler

  .weak USBHS_EP1_OUT_IRQHandler
  .thumb_set USBHS_EP1_OUT_IRQHandler,Default_Handler

  .weak USBHS_EP1_IN_IRQHandler
  .thumb_set USBHS_EP1_IN_IRQHandler,Default_Handler

  .weak SHRTIMER_IRQ0_IRQHandler
  .thumb_set SHRTIMER_IRQ0_IRQHandler,Default_Handler

  .weak SHRTIMER_IRQ1_IRQHandler
  .thumb_set SHRTIMER_IRQ1_IRQHandler,Default_Handler

  .weak CAN2_TX_IRQHandler
  .thumb_set CAN2_TX_IRQHandler,Default_Handler

  .weak CAN2_RX0_IRQHandler
  .thumb_set CAN2_RX0_IRQHandler,Default_Handler

  .weak CAN2_RX1_IRQHandler
  .thumb_set CAN2_RX1_IRQHandler,Default_Handler

  .weak CAN2_EWMC_IRQHandler
  .thumb_set CAN2_EWMC_IRQHandler,Default_Handler

  .weak I2C2_EV_IRQHandler
  .thumb_set I2C2_EV_IRQHandler,Default_Handler

  .weak I2C2_ER_IRQHandler
  .thumb_set I2C2_ER_IRQHandler,Default_Handler

  .weak USART5_IRQHandler
  .thumb_set USART5_IRQHandler,Default_Handler

  .weak I2C2_WKUP_IRQHandler
  .thumb_set I2C2_WKUP_IRQHandler,Default_Handler

  .weak USART5_WKUP_IRQHandler
  .thumb_set USART5_WKUP_IRQHandler,Default_Handler

  .weak TMU_IRQHandler
  .thumb_set TMU_IRQHandler,Default_Handler
