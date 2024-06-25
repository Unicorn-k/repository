#include "stm32f10x.h"
#include "sys.h"
#include "stdio.h"
#include "dht11.h"

//?¡é?????¨®¨º¡À
void delay_us1(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  //¡Á??o?¡§¨°?
      while(i--) ;    
   }
 }

 
//o¨¢?????¨®¨º¡À
void delay_ms1(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  //¡Á??o?¡§¨°?
      while(i--) ;    
   }
 }

//?¡ä??DHT11
void DHT11_Rst(void)
{
	DHT11_IO_OUT();//¨¦¨¨??IO?¨²?a¨º?3??¡ê¨º?
	DHT11_DQ_OUT=0;//¨¤-¦Ì¨ªDQ
	delay_ms1(20);//¨¤-¦Ì¨ª?¨¢¨¦¨´18ms
	DHT11_DQ_OUT=1;//¨¤-??DQ
	delay_us1(30);//?¡Â?¨²¨¤-??20~40us
}

//¦Ì¨¨¡äyDHT11¦Ì???¨®|
//¡¤¦Ì??1¡êo?¡ä?¨¬2a¦Ì?DHT11¦Ì?¡ä??¨²
//¡¤¦Ì??0¡êo?¨¬2a¦Ì?DHT11¦Ì?¡ä??¨²
u8 DHT11_Check(void)
{
	u8 retry=0;
	DHT11_IO_IN();//¨¦¨¨??IO?¨²?a¨º?¨¨??¡ê¨º?
	
	while(DHT11_DQ_IN&&retry<100)//DHT11?¨¢¨¤-¦Ì¨ª40~80us
	{
		retry++;delay_us1(1);
	};
	if(retry>=100)return 1;
	else retry=0;
	
	while(!DHT11_DQ_IN&&retry<100)//DHT11¨¤-¦Ì¨ªo¨®?¨¢?¨´¡ä?¨¤-??40~80us
	{
		retry++;delay_us1(1);
	};
	return 0;
}

//¡ä¨®DHT11?¨¢¨¨?¨°?????¡ê?¡¤¦Ì???¦Ì¡êo1/0
u8 DHT11_Read_Bit(void)
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//1bit¨ºy?Y¦Ì??a¨º?¡ê?¦Ì¨¨¡äy¡À??a¦Ì¨ª¦Ì???
	{
		retry++;
		delay_us1(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//1bit¨ºy?Y¦Ì??¨²¨¨Y¡ê?¦Ì¨¨¡äy¡À???¦Ì???
	{
		retry++;
		delay_us1(1);
	}
	delay_us1(40);//¦Ì¨¨¡äy40us
	
	if(DHT11_DQ_IN)return 1;//¡Á¨¹??¨¤-¦Ì¨ªo¨®¨¤-??3?1y28us¡ê??¦Ì?¡Â?¨®¨º¨¹¦Ì?¨ºy?Y?a1
	else return 0;//¡Á¨¹??¨¤-¦Ì¨ªo¨®¨¤-??2?3?1y28us¡ê??¦Ì?¡Â?¨®¨º¨¹¦Ì?¨ºy?Y?a0
}

//¡ä¨®DHT11?¨¢¨¨?¨°???¡Á??¨²
//¡¤¦Ì???¦Ì¡êo?¨¢¦Ì?¦Ì?¨ºy?Y
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

//¡ä¨®DHT11?¨¢¨¨?¨°?¡ä?¨ºy?Y
//temp:???¨¨ humi¨ºa?¨¨
//¡¤¦Ì???¦Ì¡êo0¡ê?¡À¨ª¨º??¨¢¨¨??y3¡ê
//				1¡ê?¡À¨ª¨º??¨¢¨¨?¨º¡ì¡ã¨¹
u8 DHT11_Read_Data(u8 *temp,u8 *humi,u8 *temp_little,u8 *humi_little)
{
	u8 buf[5];
	u8 i;
	DHT11_Rst();//?¡ä??DHT11
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//?¨¢¨¨?40??¨ºy?Y
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])//???y¨¨¡¤¨ºy?Y?3?¦Ì????¨®|¡À?¨¢?
		{
			*humi=buf[0];*humi_little=buf[1];
			*temp=buf[2];*temp_little=buf[3];
		}
	}
	else return 1;
	return 0;
}

//3?¨º??¡¥DHT11
u8 DHT11_Init(void)
 {
		GPIO_InitTypeDef  GPIO_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //¨º1?¨¹PA???¨²¨º¡À?¨®
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			    //PA.0 ???¨²????
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //¨ª?¨ª¨¬¨º?3?
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO?¨²?¨´?¨¨?a50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);			  	   //3?¨º??¡¥GPIOA.0
		GPIO_SetBits(GPIOA,GPIO_Pin_0);					//PA.0 ¨º?3???
	 
	 DHT11_Rst();//?¡ä??DHT11
	 return DHT11_Check();//¦Ì¨¨¡äyDHT11¦Ì???¨®|
	 
 }
