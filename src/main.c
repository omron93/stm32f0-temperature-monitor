#include "main.h"

extern long count;
extern volatile char input;
extern volatile char temperature;

extern volatile int done;
extern volatile int commands[1];

char scratchpad[9];

int main(void)
{
  LED_Config();
  
  USART1_Config();
  USART2_Config();
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  
  ConvertTemp();
  ReadTemp();
  
  if (SysTick_Config(SystemCoreClock / 125))  //2s
  { 
    /* Capture error */ 
    while (1);
  }

  /* Infinite loop */
  while (1)
    {
      if(commands[0])
        {
          switch(commands[0])
            {
              case CONVERT:
                ConvertTemp();
                ReadTemp();
                GPIOC->ODR ^= GPIO_Pin_9;
                break;
            }
        }
          
    }
}

void USART1_Config(void)
  {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*
     * USARTx configured as follow:
     *  - BaudRate = 9600 baud
     *  - Word Length = 8 Bits
     *  - Two Stop Bit
     *  - Odd parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    // Enable peripheral clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // Config pins alternate function
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    // Configure USART Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure USART Rx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART configuration
    USART_Init(USART1, &USART_InitStructure);

    // Enable USART
    USART_Cmd(USART1, ENABLE);
  }
  
void USART2_Config(void)
  {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*
     * USARTx configured as follow:
     *  - BaudRate = 9600 baud
     *  - Word Length = 8 Bits
     *  - Two Stop Bit
     *  - Odd parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    // Enable peripheral clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // Config pins alternate function
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    // Configure USART Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure USART Rx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART configuration
    USART_Init(USART2, &USART_InitStructure);

    // Enable USART
    USART_Cmd(USART2, ENABLE);
  }

void LED_Config(void)
  {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

  }

void USART2_SetSpeed(int speed)
  {
    USART_Cmd(USART2, DISABLE);
    
    USART_InitTypeDef USART_InitStructure;
  
    USART_InitStructure.USART_BaudRate = speed;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    USART_Init(USART2, &USART_InitStructure);

    // Enable USART
    USART_Cmd(USART2, ENABLE);
  }

void TReset(void)
  {
    USART2_SetSpeed(9600);
    done = 1;
    USART_SendData(USART2, 0xF0);
    while(done);
    if(input == 0xE0);
      //GPIOC->ODR ^= GPIO_Pin_8;
    
  }

void TWriteByte(char data)
  {    
    for(int i = 0; i < 8; i++)
      {
        done = 1;
        if(data & 0x01)
          USART_SendData(USART2, 0xFF);
        else
          USART_SendData(USART2, 0x00);
        
        while(done);
        data >>= 1;
      }
  }

char TReadByte(void)
  {
    char data = 0;
    
    for(int i = 0; i < 8; i++)
      {
        data >>= 1;
        done = 1;
        USART_SendData(USART2, 0xFF);
        while(done);
        if((unsigned char)input == 0xFF)
          {
            //GPIOC->ODR |= GPIO_Pin_8;
            data |= 0x80;
          }
      }
      
    return data;
  }

char TReadBit(void)
  {
    done = 1;
    USART_SendData(USART2, 0xFF);
    while(done);
    if((unsigned char)input == 0xFF)
      {
        //GPIOC->ODR |= GPIO_Pin_8;
        return 1;
      }
    return 0;
  }
  
void ConvertTemp(void)
  {
    char temp;
    
    TReset();
    USART2_SetSpeed(115200);
    TWriteByte(0xCC);
    TWriteByte(0x44);
    temp = TReadBit();
    while(temp == 0)
    {
      //GPIOC->ODR |= GPIO_Pin_9;
      temp = TReadBit();
    }
  }
  
void ReadTemp(void)
  {
    TReset();
    USART2_SetSpeed(115200);
    TWriteByte(0xCC);
    TWriteByte(0xBE);
    //char scratchpad[9];
    for(int i = 0; i < 9; i++)
      {
        scratchpad[i] = TReadByte();
      }
    temperature = 0;
    temperature |= ((scratchpad[0] >> 4) & 0x0f);
    temperature |= ((scratchpad[1] & 0x07) << 4);
    
  }
