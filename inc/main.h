#ifndef MAIN_H
#define MAIN_H

#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

void USART1_Config(void);
void USART2_Config(void);
void LED_Config(void);
void TIMER_Config(void);

void USART2_SetSpeed(int);
void TWriteByte(char);
char TReadByte(void);

#endif

