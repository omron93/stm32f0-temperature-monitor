#include "main_it.h"


int count = 0;
long c = 0;
volatile char input = 0;   //input in binary mode
volatile char temperature = 0;

int convert = 0;
int setvalues = 0;
char values[2] = {0,0};
volatile int done = 0;

void NMI_Handler(void)
  {
  }

void HardFault_Handler(void)
  {
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
      {
      }
  }

void SVC_Handler(void)
  {
  }

void PendSV_Handler(void)
  {
  }

void USART1_IRQHandler(void)
  {
  /*
   * USART stty config
   * 0:4:cbd:8a00:3:1c:7f:15:4:0:1:0:11:13:1a:0:12:f:17:16:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0
   */

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
      {
        char command = USART_ReceiveData(USART1);
        if(setvalues > 0)
          {
            values[2-setvalues] = command;
            setvalues--;
          }
        else if(command == GETTEMP)
          {
            USART_SendData(USART1, temperature);
          }
        else if(command == SETVALUES)
          {
            setvalues = 2;
          }
        else
          {
            //USART_SendData(USART1, convert);
          }
        //  GPIOC->ODR ^= GPIO_Pin_9;

        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
      }

    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
      {
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
      }
  }

void USART2_IRQHandler(void)
  {
  /*
   * USART stty config
   * 0:4:cbd:8a00:3:1c:7f:15:4:0:1:0:11:13:1a:0:12:f:17:16:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0
   */

    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
      {
        input = USART_ReceiveData(USART2);

        done = 0;
        //GPIOC->ODR ^= GPIO_Pin_8;

        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
        USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
      }

    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
      {
        USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
      }
  }

void SysTick_Handler(void)
  {
    c++;
    if(c == 2)
      {
        GPIOC->ODR ^= GPIO_Pin_8;
        if(!convert)
          {
            convert = 1;
          }
        count++;
        c = 0;
      }
  }
