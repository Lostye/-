/*****************************************************************************
*	该文件由Lostye编写
*	详细信息请查看
*	https://github.com/Lostye/Enviromental_monitoring_system.git
*****************************************************************************/

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
/*#include "OLED.h"*/
#include "Serial.h"
#include "LED.h"
#include "dht11.h"
#include "string.h"


uint32_t RxData = 00;
u8 temp=10;
u8 humi;
uint8_t Str_buf[10];

//void Num2str(uint32_t x) //将整数x转换为对应的字符串ASCII（过程中，头部的0被舍），并存储在str_buf字符数组中
//{
//    uint32_t i = 10000000, Number_Integer;
//	uint8_t bz = 0, ls;  //bz为索引
//    Number_Integer = (uint32_t)x;
//    while (Number_Integer > 0) //逐位处理
//    {
//        ls = Number_Integer / i % 10; //ls存当前位数字
//        Str_buf[bz] = (uint8_t)ls + 0x30;  //转换并逐位保存到数组
//			
//			  //以便处理下一位数字
//        Number_Integer = Number_Integer - ls * i;
//        i = i / 10;
//			
//			
//        if (ls > 0 | (ls == 0 & Str_buf[0] > 0x30)) //将头部0舍去
//  		bz++;
//    }
//}


//char * join1(char *str1, char * str2) //连接str1、str2
//{
//	char *result = (char *)malloc(strlen(str1) + strlen(str2) + 1);
//	
//	char *c = result; //c存result首地址
//	if(result == NULL) //动态分配内存不成功
//		return NULL;
//	
//	while(*str1 != '\0')
//	{
//		*result++ = *str1++;//①*result = *str1 将str1指针指向的字符赋给result指针指向的内存位置 ②str1++ 、 resul++   俩指针均各自指向后（下）一个
//	} //此时result指向自身中间位置
//	while(*str2 != '\0')
//	{
//		*result++ = *str2++;		
//	}
//	
//	return c; 
//}

int main(void)
{
	LED_Init();
	Serial_Init(); //串口通信
	LED1_ON();
	Delay_ms(5000);
	
	Serial_SendString("AT+CWMODE=2\r\n");  //设置无线模块工作在模式2（AP模式） —— 无线接入点。作为热点，其他设备可以连上它。
	while(1){
		if(Serial_GetRxData()){  //检查串口是否接收到数据（等串口接收到数据）
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
	while(1){                    //
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
	    Serial_SendString("no dht11");
		
	}
	while (1)
	{
		if(!DHT11_Read_Data(&temp,&humi)){
			Serial_SendString("AT+CIPSEND=0,31\r\n");
			Delay_ms(1500);
			int a = temp;
//			Num2str(a);
//			Serial_SendString(join1("the temp is ",Str_buf));
			int b = humi;
//			Num2str(b);
//			Serial_SendString(join1("; the humi is ",Str_buf));
//			Serial_SendString("%");
			Serial_SendString("the temp is ");
			Serial_SendNumber(a,2);
			Serial_SendString("; the humi is ");
			Serial_SendNumber(b,2);
			Serial_SendString("%\r\n");

			LED1_Turn();//状态取反
//			for(int i=0;i<10;i++){
//				Str_buf[i] = '0';
//			}
		}

		Delay_ms(5000);
	}
}

