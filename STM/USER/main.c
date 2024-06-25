/*************************************************************************************
STM32F103-30102:
	VCC<->3.3V
	GND<->GND
	SCL<->PB7
	SDA<->PB8
	IM<->PB9
0.96inch OLED :
	VCC<->3.3V
	GND<->GND
	SCL<->PA5
	SDA<->PA6
	RST<->PA3
	DC<->PA4
	CS<->PA2
USB-TTL:
	5V<->5V
	GND<->GND
	RXD<->PA9
	TXD<->PA10
**************************************************************************************/
#include "led.h"
#include "delay.h"
#include "sys.h"
//#include "usart.h"
#include "max30102.h" 
#include "myiic.h"
#include "algorithm.h"
#include "oled.h"
//#include "usartt.h"
#include "dht11.h"
#include "stdio.h"
#include "quene.h"
#include "cJSON.h"
#include "channeliic.h"
#include "stm32f10x.h"
#include "string.h"


uint32_t aun_ir_buffer[500]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;

u8 temperature;//温度值-整数部分
u8 humidity;//湿度值-整数部分
u8 temperature_little;//温度值-小数部分
u8 humidity_little;//湿度值-小数部分
float temp_result=0,hum_result=0;

int heartrate_test[30]={0};
int Spo2_test[30]={0};
int count=0;
Queue q;
//全局变量
uint8_t DefaultDatsend[15]={0xFA,0x01,0x05,0xc8,0x00,0x14,0x05,0xc8,0x00,0x14,0x01,0x00,0x0a,0x0a,0xAF};
uint8_t WorkMode0[8]={0x05,0xc8,0x00,0x14,0x05,0xc8,0x00,0x14};
uint8_t WorkMode1[8]={0x61,0x94,0x00,0x14,0x61,0x94,0x00,0x14};
uint8_t WorkMode2[8]={0x06,0x54,0x00,0x28,0x06,0x54,0x00,0x28};
uint8_t WorkMode3[8]={0x0c,0xbc,0x00,0x28,0x0c,0xbc,0x00,0x28};
uint8_t WorkMode4[8]={0x13,0x60,0x00,0x28,0x13,0x60,0x00,0x28};
uint8_t WorkMode5[8]={0x26,0xe8,0x00,0x28,0x26,0xe8,0x00,0x28};
uint8_t WorkMode6[8]={0x0f,0x78,0x00,0x28,0x0f,0x78,0x00,0x28};


#define MAX_BRIGHTNESS 255


void DHT11_DATA_upload(float *temp_result,float *hum_result);
void MAX30102_dataprocess(void);

void MA51TXX_WriteOneByte(uint8_t addr, uint8_t *nByte);
void doublechannel(uint8_t *data);
void Massage_Mode_process(int MassageMode,int current_strength_right,int current_strength_left,
	int channel_left,int channel_right,int WorkMode);
void parse_json(char * json_string);
void USART_SendString(USART_TypeDef* USARTx, char* str);



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






void MA51TXX_WriteOneByte(uint8_t addr, uint8_t *nByte)
{
	uint8_t res = 0;
	int i=0;
	
	//????
	IIC_Start_channel();
	//?????

	//??????
	IIC_Send_Byte_channel(addr);
	//????
	res = IIC_Wait_Ack_channel();
	////////////////////////////////////////////////
	
	if(res ==1)
	{
		USART_SendString(USART1,"{\r\n1111111\r\n}");
		//printf("===>falied at (%d) line\r\n",__LINE__);
		return;
	}
	//printf("hello\r\n");
	
	
	for(i = 0; i < 15; i++) {
    IIC_Send_Byte_channel(nByte[i]);
	  res = IIC_Wait_Ack_channel();
	  if(res ==1)
	  {
			USART_SendString(USART1,"{\r\n222222222\r\n}");
		  //printf("===>falied at (%d) line\r\n",__LINE__);
		  return;
	  }
		
	}
	
	
	
	//t
	IIC_Stop_channel();
	//printf("hi\r\n");
	//print_array(nByte,sizeof(nByte));
	delay_ms(10);
}



//------------------------------------cjson库-------very ok!
void parse_json(char * json_string) {
	
    cJSON *root = cJSON_Parse(json_string);
	char str[50];
	
    if (root == NULL)
    {
        //printf("Error before: [%s]\r\n", cJSON_GetErrorPtr());
			USART_SendString(USART1,"hi,error\r\n");
    }
    else
    {
        char *deviceType = cJSON_GetObjectItem(root, "deviceType")->valuestring;
        char *iotId = cJSON_GetObjectItem(root, "iotId")->valuestring;
        char *requestId = cJSON_GetObjectItem(root, "requestId")->valuestring;
        char *productKey = cJSON_GetObjectItem(root, "productKey")->valuestring;
        long long gmtCreate = cJSON_GetObjectItem(root, "gmtCreate")->valuedouble;
        char *deviceName = cJSON_GetObjectItem(root, "deviceName")->valuestring;

        cJSON *items = cJSON_GetObjectItem(root, "items");
			
        //cJSON *Led = cJSON_GetObjectItem(items, "Led");
        //long long time = cJSON_GetObjectItem(Led, "time")->valuedouble;
        //int value = cJSON_GetObjectItem(Led, "value")->valueint;
			
			//import data;
        cJSON *channel_left_json = cJSON_GetObjectItem(items, "channel_left");
        int channel_left = cJSON_GetObjectItem(channel_left_json, "value")->valueint;
			
			  cJSON *channel_right_json = cJSON_GetObjectItem(items, "channel_right");
        int channel_right = cJSON_GetObjectItem(channel_right_json, "value")->valueint;
			
			  cJSON *current_strength_left_json = cJSON_GetObjectItem(items, "current_strength_left");
        int current_strength_left = cJSON_GetObjectItem(current_strength_left_json, "value")->valueint;
			
			  cJSON *current_strength_right_json = cJSON_GetObjectItem(items, "current_strength_right");
        int current_strength_right = cJSON_GetObjectItem(current_strength_right_json, "value")->valueint;
				
				cJSON *MassageMode_json = cJSON_GetObjectItem(items, "MassageMode");
        int MassageMode = cJSON_GetObjectItem(MassageMode_json, "value")->valueint;
				
				cJSON *WorkMode_json = cJSON_GetObjectItem(items, "WorkMode");
        int WorkMode = cJSON_GetObjectItem(WorkMode_json, "value")->valueint;
				
				//----------------------------------------

        //printf("deviceType: %s\r\n", deviceType);
        //printf("iotId: %s\r\n", iotId);
        //printf("requestId: %s\r\n", requestId);
        //printf("productKey: %s\r\n", productKey);
        //printf("gmtCreate: %lld\r\n", gmtCreate);
        //printf("deviceName: %s\r\n", deviceName);
        //printf("Led time: %lld\r\n", time);
        //printf("Led value: %d\r\n", value);
			USART_SendString(USART1,productKey);
			USART_SendString(USART1,deviceType);
			USART_SendString(USART1,requestId);
			USART_SendString(USART1,productKey);
			USART_SendString(USART1,deviceName);
			USART_SendString(USART1,deviceName);
			sprintf(str,"%d\r\n",channel_left);;
			USART_SendString(USART1,str);
			sprintf(str,"%d\r\n",channel_right);;
			USART_SendString(USART1,str);
			sprintf(str,"%d\r\n",MassageMode);;
			USART_SendString(USART1,str);
			sprintf(str,"%d\r\n",current_strength_left);;
			USART_SendString(USART1,str);
			sprintf(str,"%d\r\n",current_strength_right);;
			USART_SendString(USART1,str);
			
			
			//?????在这控制不同模式
			
			if(WorkMode==1){
				Massage_Mode_process(MassageMode, current_strength_right,current_strength_left, channel_left, channel_right, WorkMode);
			}
			else {DefaultDatsend[1]=0x00;doublechannel(DefaultDatsend);}
      
			
			cJSON_Delete(root);
    }
}

void Massage_Mode_process(int MassageMode,int current_strength_right,int current_strength_left,
	int channel_left,int channel_right,int WorkMode){//模式处理
		
		
		int i=0,j=0;
		char str1[50];
		
		
		DefaultDatsend[2]=0x01;
		DefaultDatsend[10]=channel_left;
		DefaultDatsend[11]=channel_right;
		DefaultDatsend[12]=current_strength_left;
		DefaultDatsend[13]=current_strength_right;

	
	if(MassageMode==0){
		//for(int i=0;i<100;i++){MA51TXX_WriteOneByte(0x0C,DefaultDatsend);}//循环加在函数里面？
		for(i=0,j=2;i<8;i++,j++){
			DefaultDatsend[j]=WorkMode0[i];
		}
	}
	else if(MassageMode==1){
		for(i=0,j=2;i<8;i++,j++){
			DefaultDatsend[j]=WorkMode1[i];
		}
	}
	else if(MassageMode==2){
		for(i=0,j=2;i<8;i++,j++){
			DefaultDatsend[j]=WorkMode2[i];
		}
	}
	else if(MassageMode==3){
		for(i=0,j=2;i<8;i++,j++){
			DefaultDatsend[j]=WorkMode3[i];
		}
	}
	else if(MassageMode==4){
		for(i=0,j=2;i<8;i++,j++){
			DefaultDatsend[j]=WorkMode4[i];
		}
	}
	else if(MassageMode==5){
		for(i=0,j=2;i<8;i++,j++){
			DefaultDatsend[j]=WorkMode5[i];
		}
	}
	else if(MassageMode==6){
		for(i=0,j=2;i<8;i++,j++){
			DefaultDatsend[j]=WorkMode6[i];
		}
	}
	else{
	}
	for(count=0;count<15;count++){
		sprintf(str1,"%d\r\n",DefaultDatsend[count]);
		USART_SendString(USART1,str1);
	}
	
	
	//MA51TXX_WriteOneByte(0x0C,DefaultDatsend);
	doublechannel(DefaultDatsend);
		
}

void doublechannel(uint8_t *data){
	int i=0;
	for(i=0;i<300;i++){//可改------------2000
		MA51TXX_WriteOneByte(0x0C,data);
	}
}

int main(void)
{ 
	//variables to calculate the on-board LED brightness that reflects the heartbeats
	uint32_t un_min, un_max, un_prev_data;  
	int i;
	int delaytime=2,realtime=0;
//	int heartrate=0;
	int32_t n_brightness;
	float f_temp;
//	u8 temp_num=0;
	u8 temp[6];
	//u8 str[100];
	u8 dis_hr=0,dis_spo2=0;
	
	
	initQueue(&q);

	NVIC_Configuration();
	delay_init();	    	 //延时函数初始化	  
	//uart_init(115200);	 	//串口初始化为115200
	USART1_Config();
	USART2_Config();
	LED_Init();
	IIC_Init_channel();
	//进行DHT11的初始化
	 while(DHT11_Init())
	 {
		 delay_ms(1000);
			//printf("DHT11初始化失败\r\n");
	 }
	
	//OLED
	//OLED_Init();
	///OLED_ShowString(0,0,"  initializing  ",16);
	//OLED_Refresh_Gram();//更新显示到OLED	 

	max30102_init();

	//printf("\r\n MAX30102  init  \r\n");
	 
	 //-----------------------------------------------------------------------AT
	 
	 //AT+CWJAP="WIFI??","WIFI??"
	 
	 printf("AT\r\n");delay_ms1(3000);
	 //printf("AT+CWJAP=\"MMM\",\"w123456789\"\r\n");delay_ms1(3000);
	 printf("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n");delay_ms1(3000);
	 printf("AT+MQTTUSERCFG=0,1,\"\",\"Mqtt_stm32&k10gfhbhcR0\",\"16b04ac6c66cbab93ada5abc7a6166dcc2846daf9ac3d21cb541cfcc8ceb2035\",0,0,\"\"\r\n");
	 delay_ms1(3000);
	 printf("AT+MQTTCLIENTID=0,\"k10gfhbhcR0.Mqtt_stm32|securemode=2\\,signmethod=hmacsha256\\,timestamp=1713976145854|\"\r\n");
	 delay_ms1(3000);
	 printf("AT+MQTTCONN=0,\"iot-06z00cikkaqmojm.mqtt.iothub.aliyuncs.com\",1883,1\r\n");
	 delay_ms1(3000);
	 
	 
	 
	 
	 
	 
	 
	 
	 //-----------------------------------------------------------------------AT

	un_min=0x3FFFF;
	un_max=0;
	
	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
	//read the first 500 samples, and determine the signal range
    for(i=0;i<n_ir_buffer_length;i++)
    {
        while(MAX30102_INT==1);   //wait until the interrupt pin asserts
        
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
		aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
            
        if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];    //update signal min
        if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];    //update signal max
    }
	un_prev_data=aun_red_buffer[i];
	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	
	while(1)
	{
		
		if(count<30) MAX30102_dataprocess();
		else{
		i=0;
        un_min=0x3FFFF;
        un_max=0;
		
		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            //update the signal min and max
            if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];
            if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];
        }
		//take 100 sets of samples before calculating the heart rate.
        for(i=400;i<500;i++)
        {
            un_prev_data=aun_red_buffer[i-1];
            while(MAX30102_INT==1);
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
			aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
        
            if(aun_red_buffer[i]>un_prev_data)
            {
                f_temp=aun_red_buffer[i]-un_prev_data;
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness-=(int)f_temp;
                if(n_brightness<0)
                    n_brightness=0;
            }
            else
            {
                f_temp=un_prev_data-aun_red_buffer[i];
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness+=(int)f_temp;
                if(n_brightness>MAX_BRIGHTNESS)
                    n_brightness=MAX_BRIGHTNESS;
            }
			//send samples and calculation result to terminal program through UART
			if(ch_hr_valid == 1 && ch_spo2_valid ==1 && n_heart_rate<130 && n_sp02<102)//**/ ch_hr_valid == 1 && ch_spo2_valid ==1 && n_heart_rate<120 && n_sp02<101
			{
				dis_hr = n_heart_rate;
				dis_spo2 = n_sp02;
			}
			/*
			else
			{
				dis_hr = 0;
				dis_spo2 = 0;
			}*/
				//printf("HR=%i, ", n_heart_rate); 
				//printf("HRvalid=%i, ", ch_hr_valid);
				//printf("SpO2=%i, ", n_sp02);
				//printf("SPO2Valid=%i\r\n", ch_spo2_valid);
			
		}
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		
		
		dequeue(&q);//出队
		enqueue(&q,dis_hr);//入队
		
		//printf("心率2：%d\r\n",dis_hr);
		//printQueue(&q);
		//printf("均值：%d\r\n",average(&q));//传这个
		dis_hr=average(&q);
		
		
		//}
		//---------------------------队列
		
		//-----------------------------
		
		//if count>20
		//---------------------------
		DHT11_DATA_upload(&temp_result,&hum_result);
		//printf("温度：%.2f \t 湿度：%.2f\r\n",temp_result,hum_result);
		
		//显示刷新
		
		printf("AT+MQTTPUB=0,\"/sys/k10gfhbhcR0/Mqtt_stm32/thing/event/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"version\\\":\\\"1.0\\\"\\,\\\"params\\\":{\\\"temp\\\":%.2f\\,\\\"hum\\\":%.2f\\,\\\"Led\\\":0\\,\\\"heartrate\\\":%d\\,\\\"spo2\\\":%d}\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",0,0\r\n",temp_result,hum_result,dis_hr,dis_spo2);
	
		if(realtime>=delaytime){
			//printf("AT+MQTTPUB=0,\"/sys/k10gfhbhcR0/Mqtt_stm32/thing/event/property/post\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"version\\\":\\\"1.0\\\"\\,\\\"params\\\":{\\\"temp\\\":%.2f\\,\\\"hum\\\":%.2f\\,\\\"Led\\\":0\\,\\\"heartrate\\\":%d\\,\\\"spo2\\\":%d}\\,\\\"method\\\":\\\"thing.event.property.post\\\"}\",0,0\r\n",temp_result,hum_result,dis_hr,dis_spo2);
			realtime=0;
			}
			//printf("realtime:%d,delaytime:%d\r\n",realtime,delaytime);
		
			realtime+=2;
			delay_ms(2);
		
		
		//OLED_ShowString(0,0,str,16);
		//OLED_Fill(0,23,127,63,0);
		//红光在上，红外在下
		//dis_DrawCurve(aun_red_buffer,20);
		//dis_DrawCurve(aun_ir_buffer,0);
		//OLED_Refresh_Gram();//更新显示到OLED
		}			
	}
}
/*
void dis_DrawCurve(u32* data,u8 x)
{
	u16 i;
	u32 max=0,min=262144;
	u32 temp;
	u32 compress;
	
	for(i=0;i<128*2;i++)
	{
		if(data[i]>max)
		{
			max = data[i];
		}
		if(data[i]<min)
		{
			min = data[i];
		}
	}
	
	compress = (max-min)/20;
	
	for(i=0;i<128;i++)
	{
		temp = data[i*2] + data[i*2+1];
		temp/=2;
		temp -= min;
		temp/=compress;
		if(temp>20)temp=20;
		OLED_DrawPoint(i,63-x-temp,1);
	}
}
*/
void DHT11_DATA_upload(float *temp_result,float *hum_result){
	 
	 
	 //读取数据并通过串口助手显示出来
	 
		 DHT11_Read_Data(&temperature,&humidity,&temperature_little,&humidity_little);//读取数据
		 
		 *temp_result=(float)(temperature+temperature_little*0.1);
		 *hum_result=(float)(humidity+humidity_little*0.1);
		 //printf("温度：%.2f \t 湿度：%.2f\r\n",temp,hum);
	
		 
	 //上传？？？
	//printf("AT+MQTTPUB=0,\"/sys/k10gfhbhcR0/Mqtt_stm32/thing/event/property/post\",\"{\\\"params\\\":{\\\"temp\\\":%.2f}}\",0,0\r\n",temp);
		 //delay_ms(3000);
	//printf("AT+MQTTPUB=0,\"/sys/k10gfhbhcR0/Mqtt_stm32/thing/event/property/post\",\"{\\\"params\\\":{\\\"hum\\\":%.2f}}\",0,0\r\n",hum);
		 //delay_ms(3000);
	
	
	//AT+MQTTPUB=0,"/sys/k10gfhbhcR0/Mqtt_stm32/thing/event/property/post",
	//"{\"id\":\"123\"\,\"version\":\"1.0\"\,\"params\":{\"temp\":54.6\,\"hum\":76.1\,\"Led\":1}\,\"method\":\"thing.event.property.post\"}",1,0
} 

void MAX30102_dataprocess(){
	int i=0;
	u8 temp[6];
	uint32_t un_min, un_max, un_prev_data;  
	int32_t n_brightness;
	u8 dis_hr=0,dis_spo2=0;
	float f_temp;
        un_min=0x3FFFF;
        un_max=0;
		
		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            //update the signal min and max
            if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];
            if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];
        }
		//take 100 sets of samples before calculating the heart rate.
        for(i=400;i<500;i++)
        {
            un_prev_data=aun_red_buffer[i-1];
            while(MAX30102_INT==1);
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
			aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
        
            if(aun_red_buffer[i]>un_prev_data)
            {
                f_temp=aun_red_buffer[i]-un_prev_data;
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness-=(int)f_temp;
                if(n_brightness<0)
                    n_brightness=0;
            }
            else
            {
                f_temp=un_prev_data-aun_red_buffer[i];
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness+=(int)f_temp;
                if(n_brightness>MAX_BRIGHTNESS)
                    n_brightness=MAX_BRIGHTNESS;
            }
			//send samples and calculation result to terminal program through UART
			if(ch_hr_valid == 1 && ch_spo2_valid ==1 && n_heart_rate<130 && n_sp02<102)//**/ ch_hr_valid == 1 && ch_spo2_valid ==1 && n_heart_rate<120 && n_sp02<101
			{
				dis_hr = n_heart_rate;
				dis_spo2 = n_sp02;
			}
			/*
			else
			{
				dis_hr = 0;
				dis_spo2 = 0;
			}*/
				//printf("HR=%i, ", n_heart_rate); 
				//printf("HRvalid=%i, ", ch_hr_valid);
				//printf("SpO2=%i, ", n_sp02);
				//printf("SPO2Valid=%i\r\n", ch_spo2_valid);
			
		}
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		
		heartrate_test[count]=dis_hr;
		//printf("心率1：%d\r\n",dis_hr);
		//Spo2_test[count]=dis_spo2;
		count++;
		if(count==30){
			int modeCount = 0;
      int mode = findMode(heartrate_test, SIZE, &modeCount);
      if (modeCount == 1) {
          mode = findMedian(heartrate_test, SIZE);
      }
      findGreaterNumbers(&q, heartrate_test, SIZE, mode);
      findSmallerNumbers(&q, heartrate_test, SIZE, mode);
      enqueue(&q, mode);
		}
		
		//}
		//---------------------------队列
		
		//-----------------------------
		
		//if count>20
		//---------------------------
}




