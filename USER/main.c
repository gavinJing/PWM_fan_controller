#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	
#include "usmart.h"	 
#include "rtc.h" 
#include "adc.h"
#include "ds18b20.h" 

/************************************************
 ALIENTEK ս��STM32F103������ʵ��15
 RTCʵʱʱ��ʵ��  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

 int main(void)
 {
  short temperature;  
  u8 NTC_temperature=0;		
	u32 current_t=0;
	u32 previous_t=0;
	uc8 interval=1;
 	u8 t=0;	
	u16 adcx;
	float temp;
	const int temp_A=10;
	u8 temp_position=0;
	const int TempSize=93;
  
  //temp_position = temperature_table;	 
	const u16 temperature_table[TempSize]={3891,3820,3748,3676,3603,3531,3458,3386,3314,3242,3171,3100,3029,2959,2890,2821,2753,2686,2619,2554,2489,2426,2363,2302,2241,2182,2123,2066,2010,1955,1902,1849,1798,1748,1698,1651,1604,1558,1514,1471,1429,1388,1348,1309,1217,1234,1198,1164,1130,1097,1065,1034,1004,975,947,919,893,867,842,817,794,771,749,771,749,727,706,686,667,648,629,611,594,577,561,545,530,515,501,487,474,461,448,436,424,412,401,390,380,370,360,350,341};
	
	u16 *pTmp ;     
  pTmp=(uint16_t *)temperature_table ;
  
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();	
  Adc_Init();		  		//ADC��ʼ��
	 
	usmart_dev.init(SystemCoreClock/1000000);	//��ʼ��USMART	
	RTC_Init();	  			//RTC��ʼ��
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"PWM fan controller");	
	LCD_ShowString(60,90,200,16,16,"By Jack");
	//LCD_ShowString(60,110,200,16,16,"2015/1/14");		
	//��ʾʱ��
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(60,130,200,16,16,"    -  -  ");	   
	LCD_ShowString(60,162,200,16,16,"  :  :  ");
	LCD_ShowString(60,180,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(60,200,200,16,16,"ADC_CH0_VOL:0.000V");	
	LCD_ShowString(60,240,200,16,16,"Temp:   . C");
	LCD_ShowString(60,260,200,16,16,"NTC Temp:     C");
	while(DS18B20_Init())	//DS18B20��ʼ��	
	{
		LCD_ShowString(30,220,200,16,16,"DS18B20 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}								   
	LCD_ShowString(60,220,200,16,16,"DS18B20 OK");
	

	while(1)
	{		
		if(current_t-previous_t>=interval)			//set 1 second interval to read the ADC value and temperature
		{		
      adcx=Get_Adc_Average(ADC_Channel_1,10);
		  LCD_ShowxNum(156,180,adcx,4,16,0);//��ʾADC��ֵ
		  temp=(float)adcx*(3.3/4096);
		  adcx=temp;
		  LCD_ShowxNum(156,200,adcx,1,16,0);//��ʾ��ѹֵ
		  temp-=adcx;
		  temp*=1000;
		  LCD_ShowxNum(172,200,temp,3,16,0X80);			
			temperature=DS18B20_Get_Temp();	
			
			temp=Get_Adc_Average(ADC_Channel_1,10);
			temp_position=Fine_data_position(pTmp,TempSize,temp);
			NTC_temperature=Get_RTC_Temperature(temp_A,temp_position);
			
			
			if(temperature<0)
			{
				LCD_ShowChar(30+40,240,'-',16,0);			//��ʾ����
				temperature=-temperature;					//תΪ����
			}else LCD_ShowChar(30+40,240,' ',16,0);			//ȥ������
			LCD_ShowNum(30+40+8+30,240,temperature/10,2,16);	//��ʾ��������	    
   		LCD_ShowNum(30+40+32+30,240,temperature%10,1,16);	//��ʾС������
			LCD_ShowNum(30+40+32+30,260,NTC_temperature,2,16);
			
      previous_t=current_t;
		}				   
	 	delay_ms(10);
    current_t=RTC_GetCounter();     
		
		
		//LED0=!LED0;
		//delay_ms(250);		
		if(t!=calendar.sec)
		{
			t=calendar.sec;
			//previous_t=t;
			LCD_ShowNum(60,130,calendar.w_year,4,16);									  
			LCD_ShowNum(100,130,calendar.w_month,2,16);									  
			LCD_ShowNum(124,130,calendar.w_date,2,16);
			
			switch(calendar.week)
			{
				case 0:
					LCD_ShowString(60,148,200,16,16,"Sunday   ");
					break;
				case 1:
					LCD_ShowString(60,148,200,16,16,"Monday   ");
					break;
				case 2:
					LCD_ShowString(60,148,200,16,16,"Tuesday  ");
					break;
				case 3:
					LCD_ShowString(60,148,200,16,16,"Wednesday");
					break;
				case 4:
					LCD_ShowString(60,148,200,16,16,"Thursday ");
					break;
				case 5:
					LCD_ShowString(60,148,200,16,16,"Friday   ");
					break;
				case 6:
					LCD_ShowString(60,148,200,16,16,"Saturday ");
					break;  
			}
			LCD_ShowNum(60,162,calendar.hour,2,16);									  
			LCD_ShowNum(84,162,calendar.min,2,16);									  
			LCD_ShowNum(108,162,calendar.sec,2,16);
			LED0=!LED0;
		}	
		delay_ms(10);								  
	};  
 }

