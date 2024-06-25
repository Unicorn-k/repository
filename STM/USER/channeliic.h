
#ifndef __CHANNELIIC_H
#define __CHANNELIIC_H
 
#include "sys.h"
#include "stm32f10x.h"
 
//IO????	 
#define IIC_SCL_CHANNEL    PBout(10) //SCL
#define IIC_SDA_CHANNEL    PBout(11) //SDA	 
#define READ_SDA_CHANNEL   PBin(11)  //??SDA 
 
//IIC??????
void IIC_Init_channel(void);                //???IIC?IO?				 
void IIC_Start_channel(void);				//??IIC????
void IIC_Stop_channel(void);	  			//??IIC????
void IIC_Send_Byte_channel(u8 txd);			//IIC??????
u8 IIC_Read_Byte_channel(unsigned char ack);//IIC??????
u8 IIC_Wait_Ack_channel(void); 				//IIC??ACK??
void IIC_Ack_channel(void);					//IIC??ACK??
void IIC_NAck_channel(void);				//IIC???ACK??
 
//void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif


