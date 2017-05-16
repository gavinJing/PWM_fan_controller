#include "adc.h"
#include "delay.h"


const int temp_A =10;
//const int16_t temperature_table[89]={3891,3820,3748,3676,3603,3531,3458,3386,3314,3242,3171,3100,3029,2959,2890,2821,2753,2686,2619,2554,2489,2426,2363,2302,2241,2182,2123,2066,2010,1955,1902,1849,1798,1748,1698,1651,1604,1558,1514,1471,1429,1388,1348,1309,1217,1234,1198,1164,1130,1097,1065,1034,1004,975,947,919,893,867,842,817,794,771,749,771,749,727,706,686,667,648,629,611,594,577,561,545,530,515,501,487,474,461,448,436,424,412,401,390,380,370,360,350,341};


//const int16_t temperature_table[4]={3891,3820,3748,3676};
//int16_t *pTmp; 
     
//int16_t pTmp = temperature_table ; 
	
void Adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //set PWM output at PA1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //set PWM output at PA1

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	ADC_DeInit(ADC1);  //��λADC1 
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼���� 
	
	
}


u16  Get_Adc(u8 ch)
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
	
	
}


u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
	
}

u8 Fine_data_position(u16 *a,u8 ArrayLong,u16 data)//binary searching,
{  
    u16 begin,end,middle ;  
    u8 i ;  
    begin = 0 ;  
    end = ArrayLong-1 ;  
    i = 0  ;  
    if(data >= a[begin]) return begin ;  
    else if(data <= a[end]) return end ;  
    while(begin < end)  
    {  
        middle = (begin+end)/2 ;  
        if(data == a[middle] ) break ;  
        if(data < a[middle] && data > a[middle+1]) break ;   
        if(data > a[middle])  end = middle ;                      
        else begin = middle ;      
        if(i++ > ArrayLong) break ;  
    }  
    if(begin > end ) return 0 ;   
    return middle ;  
}  

u16 Get_RTC_Temperature(const int temp_A, u8 ArrayPosition) 
{
	u8 temperature=0;
	temperature=temp_A+ArrayPosition;  
	return temperature;
	
}
