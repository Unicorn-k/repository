#include "channeliic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//㊣?3足D辰??1??∫?～那1車?㏒??∩?-℅¯??D赤?谷㏒?2?米?車?車迆???邦豕?o?車?赤?
//Mini STM32?a﹞⊿～?
//IIC ?y?‘o‘那y	   
//?y米??-℅車@ALIENTEK
//??那???足3:www.openedv.com
//DT??豕??迆:2010/6/10 
//～?㊣?㏒oV1.0
//～?豕“?迄車D㏒?米芍～?㊣????㏒
//Copyright(C) ?y米??-℅車 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


 
 
//
//???IIC
void IIC_Init_channel(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); // ??PB????	
 
	
	//PB8-SCL  PB9-SDA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	//???????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//???PC??
	GPIO_SetBits(GPIOB, GPIO_Pin_10);   // ????LED
	GPIO_SetBits(GPIOB, GPIO_Pin_11);   // ????LED
 
	
}
 
//IO????
void SDA_IN_channel()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//PB9-SDA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//???????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
void SDA_OUT_channel()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//PB9-SDA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//???????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
 
//??IIC????
void IIC_Start_channel(void)
{
	SDA_OUT_channel();     //sda???
	IIC_SDA_CHANNEL=1;	  	  
	IIC_SCL_CHANNEL=1;
	delay_us(4);
 	IIC_SDA_CHANNEL=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_CHANNEL=0;//??I2C??,????????? 
}	  
 
//??IIC????
void IIC_Stop_channel(void)
{
	SDA_OUT_channel();//sda???
	IIC_SCL_CHANNEL=1;
	IIC_SDA_CHANNEL=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_CHANNEL=1; 
	IIC_SDA_CHANNEL=1;//??I2C??????
	delay_us(4);							   	
}
 
//????????
//???:1,??????
//        0,??????
u8 IIC_Wait_Ack_channel(void)
{
	u8 ucErrTime=0;
	SDA_IN_channel();      //SDA?????  
	IIC_SDA_CHANNEL=1;delay_us(1);	   
	IIC_SCL_CHANNEL=1;delay_us(1);	 
	while(READ_SDA_CHANNEL)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop_channel();
			return 1;
		}
	}
	IIC_SCL_CHANNEL=0;//????0 	   
	return 0;  
} 
 
//??ACK??
void IIC_Ack_channel(void)
{
	IIC_SCL_CHANNEL=0;
	SDA_OUT_channel();
	IIC_SDA_CHANNEL=0;
	delay_us(2);
	IIC_SCL_CHANNEL=1;
	delay_us(2);
	IIC_SCL_CHANNEL=0;
}
 
//???ACK??		    
void IIC_NAck_channel(void)
{
	IIC_SCL_CHANNEL=0;
	SDA_OUT_channel();
	IIC_SDA_CHANNEL=1;
	delay_us(2);
	IIC_SCL_CHANNEL=1;
	delay_us(2);
	IIC_SCL_CHANNEL=0;
}
 
//IIC??????
//????????
//1,???
//0,???			  
void IIC_Send_Byte_channel(u8 txd)
{                        
    u8 t;   
	SDA_OUT_channel(); 	    
    IIC_SCL_CHANNEL=0;//??????????
    for(t=0;t<8;t++)
    {              
        IIC_SDA_CHANNEL=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //?TEA5767??????????
		IIC_SCL_CHANNEL=1;
		delay_us(2); 
		IIC_SCL_CHANNEL=0;	
		delay_us(2);
    }	 
}
 
//?1???,ack=1?,??ACK,ack=0,??nACK   
u8 IIC_Read_Byte_channel(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN_channel();//SDA?????
    for(i=0;i<8;i++ )
	{
        IIC_SCL_CHANNEL=0; 
        delay_us(2);
		IIC_SCL_CHANNEL=1;
        receive<<=1;
        if(READ_SDA_CHANNEL)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck_channel();//??nACK
    else
        IIC_Ack_channel(); //??ACK   
    return receive;
}
