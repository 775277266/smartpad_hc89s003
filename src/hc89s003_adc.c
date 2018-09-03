/*
* �ļ�˵����hc89s003f4 adcģ�����
*/
#include "hc89s003_adc.h"
#include "HC89S003F4.h"
#include "hc89s003_int.h"

/*
* ���룺
* �����
* ���ܣ�ADCģ���ʼ�����ڲ��ο���ѹ2.7V��ת��ʱ��500k��
*/
void adcConfig(void)
{
    ENABLE_ADC();
    
    ADCC0 |= INREF_2V;
    ADCC2 |= ADCL_12BITS + ALIGN_1 + ADCTS_1M + ADC_CLK_FOSC_8;
	
	ENABLE_ADC_INT();
}

/*
* ���룺
* �����
* ���ܣ�����ADC���ֵ
*/
uint16_t getAdcResult(AdcResult_t t)
{
    uint16_t v = ADCRH & (uint8_t)t;
    v <<= 8;
    v += ADCRL;
	
	return v;
}


// end of file
