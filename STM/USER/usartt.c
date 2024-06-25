#include "stm32f10x.h"
#include "sys.h"
#include "stdio.h"
#include "usartt.h"
#include "string.h"

//-------------------

#define RX_BUF_SIZE 2000
char RX_BUF[RX_BUF_SIZE];
uint8_t json_start_flag = 0;
uint8_t json_end_flag = 0;
uint8_t brace_count = 0;
uint16_t RX_WR_Index = 0;



//------------------------

void parse_json(char * json_string);

void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void USART2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        char temp = USART_ReceiveData(USART1);
        USART_SendData(USART2, temp);
    }
}
/*
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        char temp = USART_ReceiveData(USART2);
        //USART_SendData(USART1, temp);
    }
}*/

void remove_substring(char* s, const char* to_remove)
{
	size_t len_up_to_match;
    size_t to_remove_len = strlen(to_remove);
    char* match = strstr(s, to_remove);
    while(match != NULL)
    {
        char* end = strchr(match + to_remove_len, ',');
        if(end != NULL)
        {
            to_remove_len = end - match + 1; // ????????,???????
        }
        len_up_to_match = match - s;
        strcpy(s + len_up_to_match, match + to_remove_len);
        match = strstr(s, to_remove);
    }
}
//--------去除内部就ok'-----------------ok了
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        char data = USART_ReceiveData(USART2);
        if(data == '{') 
        {
            json_start_flag = 1;
            brace_count++;
            if(brace_count == 1) // ??????'{',????JSON????,??RX_WR_Index
            {
                RX_WR_Index = 0;
            }
        }
        if(json_start_flag)
        {
            RX_BUF[RX_WR_Index % RX_BUF_SIZE] = data;
            RX_WR_Index++;
            if(data == '}')
            {
                brace_count--;
                if(brace_count == 0)
                {
                    json_end_flag = 1;
                    json_start_flag = 0;
                    RX_BUF[RX_WR_Index % RX_BUF_SIZE] = '\0'; // ????????
                }
            }
        }
        if(json_end_flag)
        {
            
					remove_substring(RX_BUF, "\r\n+MQTTSUBRECV:0,\"/sys/k10gfhbhcR0/Mqtt_stm32/thing/service/property/set\",");
					USART_SendString(USART1,RX_BUF);
					parse_json(RX_BUF);
            json_end_flag = 0;
        }
    }
}


//以下20行是printf串口调试所需程序
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle;
}; 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//等待发送完毕   
  USART2->DR = (u8) ch;  //发送单个字符    
	return ch;
}

void USART_SendData_u8(USART_TypeDef* USARTx, u8 Data)
{
    /* ????????????? */
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

    /* ???????????? */
    USART_SendData(USARTx, Data);
}
void USART_SendString(USART_TypeDef* USARTx, char* str)
{
    while(*str != '\0')  // ????????????
    {
        USART_SendData_u8(USARTx, *str);  // ??????
        str++;  // ????????
    }
} 
 

