#include "sys.h"

//IO��??������??
#define DHT11_IO_IN() {GPIOA->CRL&=0xFFFFFFF0;GPIOA->CRL|=8<<0;}//����??IO?��?a��?��??�꨺?
#define DHT11_IO_OUT() {GPIOA->CRL&=0xFFFFFFF0;GPIOA->CRL|=3<<0;}//����??IO?��?a��?3??�꨺?

//IO2������
#define DHT11_DQ_OUT PAout(0)//��y?Y???��
#define DHT11_DQ_IN PAin(0)//��y?Y???��

void delay_us1(u16 time);
void delay_ms1(u16 time);
void DHT11_Rst(void);
u8 DHT11_Check(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi,u8 *temp_little,u8 *humi_little);
u8 DHT11_Init(void);
