/*****************************************************************************
*	该文件由Lostye编写
*	详细信息请查看
*	https://github.com/Lostye/Enviromental_monitoring_system.git
*****************************************************************************/

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "dht11.h"
#include "string.h"


uint32_t RxData = 00;
u8 temp=10;
u8 humi;
uint8_t Str_buf[10];

void Num2str(uint32_t x)
{
    uint32_t i = 10000000, Number_Integer;
    uint8_t bz = 0, ls;
    Number_Integer = (uint32_t)x;
    while (Number_Integer > 0)
    {
        ls = Number_Integer / i % 10;
        Str_buf[bz] = (uint8_t)ls + 0x30;
        Number_Integer = Number_Integer - ls * i;
        i = i / 10;
        if (ls > 0 | (ls == 0 & Str_buf[0] > 0x30)) //将头部0舍去
  		bz++;
    }
}


char * join1(char *str1, char * str2)
{
	char *result = (char *)malloc(strlen(str1) + strlen(str2) + 1);
	
	char *c = result;
	if(result == NULL)
		return NULL;
	
	while(*str1 != '\0')
	{
		*result++ = *str1++;		
	}
	while(*str2 != '\0')
	{
		*result++ = *str2++;		
	}
	
	return c;
}

int main(void)
{
	LED_Init();
	Serial_Init();
	LED1_ON();
	Delay_ms(5000);
	Serial_SendString("AT+CWMODE=2\r\n");
	while(1){
		if(Serial_GetRxData()){
			break;
		}
	}
	Delay_ms(1000);
	Serial_SendString("AT+RST\r\n");
	while(1){
		if(Serial_GetRxData()){
			break;
		}
	}
	Delay_ms(1000);
	Serial_SendString("AT+CIPMUX=1\r\n");
	while(1){
		if(Serial_GetRxData()){
			break;
		}
	}
	Delay_ms(1000);
	Serial_SendString("AT+CIPSERVER=1,8080\r\n");
	while(1){
		if(Serial_GetRxData()){
			break;
		}
	}
	Delay_ms(1000);
	while(DHT11_Init())
	{
		printf("DHT11 Error \r\n");
		Serial_SendString("AT+CIPSEND=0,25\r\n");
		Delay_ms(1500);
	    Serial_SendString("xiaolang say no dht11");
	}
	while (1)
	{
		LED1_Turn();
		if(DHT11_Read_Data(&temp,&humi)){
			Serial_SendString("AT+CIPSEND=0,30\r\n");
			Delay_ms(1500);
			int a = temp;
			Num2str(a);
			Serial_SendString(join1("the temp is ",Str_buf));
			int b = humi;
			Num2str(b);
			Serial_SendString(join1("; the humi is ",Str_buf));
			
		}

		Delay_ms(5000);
	}
}

