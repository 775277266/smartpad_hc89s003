/*
* �ļ�˵��������ȷ��ʱ����
*/
#include "hc89s003_delay.h"

/*
* ���룺n-��ʱʱ��
* �����
* ���ܣ�Fcpu = 16MHz(��Ӧm=1596)��n = 1ʱ����ʱʱ��ԼΪ1Ms
*/
void delay_ms(uint16_t n)
{
	unsigned int m;
	for(;n > 0;n --)
	{
		for(m = 399;m > 0;m --);
	}
}

/*
* ���룺n-��ʱʱ��
* �����
* ���ܣ�Fcpu = 16MHz, n = 1ʱ����ʱʱ��ԼΪ2us
*       Fcpu = 4MHz��������ʱ8us
*/
void delay_us(uint16_t n)
{
	while(n--);	
}



// end of file
