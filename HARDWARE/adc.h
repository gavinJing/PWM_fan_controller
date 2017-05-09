#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"



void Adc_Init(void);
u16 Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
u8 Fine_data_position(u16 *a,u8 ArrayLong,u16 data);
u8 Get_RTC_Temperature(uc temp_A, u16 ArrayPosition);
 
#endif 