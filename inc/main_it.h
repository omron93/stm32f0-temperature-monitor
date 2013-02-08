#ifndef MAIN_IT_H
#define MAIN_IT_H

#include "main.h"

  void NMI_Handler(void);
  void HardFault_Handler(void);
  void SVC_Handler(void);
  void PendSV_Handler(void);
  void SysTick_Handler(void);
  void USART1_IRQHandler(void);
  void USART2_IRQHandler(void);
  void EXTI0_1_IRQHandler(void);
  void TIM3_IRQHandler(void);

#endif
