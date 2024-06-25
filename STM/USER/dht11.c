#include "stm32f10x.h"
#include "sys.h"
#include "stdio.h"
#include "dht11.h"

//?��?????������
void delay_us1(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  //��??o?����?
      while(i--) ;    
   }
 }

 
//o��?????������
void delay_ms1(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  //��??o?����?
      while(i--) ;    
   }
 }

//?��??DHT11
void DHT11_Rst(void)
{
	DHT11_IO_OUT();//����??IO?��?a��?3??�꨺?
	DHT11_DQ_OUT=0;//��-�̨�DQ
	delay_ms1(20);//��-�̨�?������18ms
	DHT11_DQ_OUT=1;//��-??DQ
	delay_us1(30);//?��?����-??20~40us
}

//�̨���yDHT11��???��|
//����??1��o?��?��2a��?DHT11��?��??��
//����??0��o?��2a��?DHT11��?��??��
u8 DHT11_Check(void)
{
	u8 retry=0;
	DHT11_IO_IN();//����??IO?��?a��?��??�꨺?
	
	while(DHT11_DQ_IN&&retry<100)//DHT11?����-�̨�40~80us
	{
		retry++;delay_us1(1);
	};
	if(retry>=100)return 1;
	else retry=0;
	
	while(!DHT11_DQ_IN&&retry<100)//DHT11��-�̨�o��?��?����?��-??40~80us
	{
		retry++;delay_us1(1);
	};
	return 0;
}

//�䨮DHT11?����?��?????��?����???�̡�o1/0
u8 DHT11_Read_Bit(void)
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//1bit��y?Y��??a��?��?�̨���y��??a�̨���???
	{
		retry++;
		delay_us1(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//1bit��y?Y��??����Y��?�̨���y��???��???
	{
		retry++;
		delay_us1(1);
	}
	delay_us1(40);//�̨���y40us
	
	if(DHT11_DQ_IN)return 1;//����??��-�̨�o����-??3?1y28us��??��?��?��������?��y?Y?a1
	else return 0;//����??��-�̨�o����-??2?3?1y28us��??��?��?��������?��y?Y?a0
}

//�䨮DHT11?����?��???��??��
//����???�̡�o?����?��?��y?Y
u8 DHT11_Read_Byte(void)
{
	u8 i,dat;
	dat=0;
	for(i=0;i<8;i++)
	{
		dat<<=1;
		dat|=DHT11_Read_Bit();
	}
	return dat;
}

//�䨮DHT11?����?��?��?��y?Y
//temp:???�� humi��a?��
//����???�̡�o0��?������??����??y3��
//				1��?������??����?����㨹
u8 DHT11_Read_Data(u8 *temp,u8 *humi,u8 *temp_little,u8 *humi_little)
{
	u8 buf[5];
	u8 i;
	DHT11_Rst();//?��??DHT11
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//?����?40??��y?Y
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])//???y������y?Y?3?��????��|��?��?
		{
			*humi=buf[0];*humi_little=buf[1];
			*temp=buf[2];*temp_little=buf[3];
		}
	}
	else return 1;
	return 0;
}

//3?��??��DHT11
u8 DHT11_Init(void)
 {
		GPIO_InitTypeDef  GPIO_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //��1?��PA???������?��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			    //PA.0 ???��????
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //��?������?3?
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO?��?��?��?a50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);			  	   //3?��??��GPIOA.0
		GPIO_SetBits(GPIOA,GPIO_Pin_0);					//PA.0 ��?3???
	 
	 DHT11_Rst();//?��??DHT11
	 return DHT11_Check();//�̨���yDHT11��???��|
	 
 }
