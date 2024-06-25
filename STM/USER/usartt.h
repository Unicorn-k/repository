#include "stdio.h"	
#include "sys.h" 



void USART1_Config(void);
void USART2_Config(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART_SendData_u8(USART_TypeDef* USARTx, u8 Data);
void USART_SendString(USART_TypeDef* USARTx, char* str);


