#include "main_it.h"


int count = 0;
long c = 0;
char input = 0;   //input in binary mode
char response = 0;      //response

volatile int done = 0;
volatile char zprava = -1;

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

void SysTick_Handler(void)
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
        //char in = (USART_ReceiveData(USART1));
        USART_ReceiveData(USART1);

        //if(in == 255)
        //  GPIOC->ODR ^= GPIO_Pin_9;

        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
        
        //USART_SendData(USART2, 0xF0);
        USART_SendData(USART1, input);
        USART_SendData(USART1, zprava);

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

void TIM3_IRQHandler(void)
{
  c++;
  if(c == 100)
    {
      //GPIOC->ODR ^= GPIO_Pin_8;
      c = 0;
    }
  count++;
  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
}
