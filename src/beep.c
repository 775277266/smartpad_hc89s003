/*
* �ļ�˵������Դ����������
*/
#include "HC89S003F4.h"
#include "beep.h"
#include "tick.h"
#include "hc89s003_pwm.h"
#include "hc89s003_gpio.h"

static uint16_t buzzCounter = 0;	// ��������������еĴ���
static uint16_t startTick = 0;		// ���е�ʱ���
static uint16_t beepOnInterval = 0xff;
static uint16_t beepOffInterval = 0xff;
static uint8_t  buzzStatus = 0;		// 0-��ǰ���ڲ�����״̬����ǰ���ڷ���״̬

#define BEEP_PIN	P0_0


#define BEEP_OFF()	do{PWM3C &= ~(PWM3_EN | PWM3_OUT_EN); BEEP_PIN = 0;}while(0)
#define BEEP_ON()	do{PWM3C |= (PWM3_EN | PWM3_OUT_EN);}while(0)


/*
 ������n--���������еĴ�����0xffff��ʾ���������; onInterval--������ʱ����offInterval--��������ʱ��(��λ100ms)
 ���أ�
 ���ܣ���������ʼ��������У�
*/
void beepStartBuzz(uint16_t n, uint16_t onInterval, uint16_t offInterval)
{
	// �������Ѵ��ڳ���״̬�����ٽ����µĿ���
	if(buzzCounter != 0xffff)
	{
		BEEP_ON();
		startTick = getTick();
		buzzCounter = n;
		buzzStatus = 1;
		beepOnInterval = onInterval;
		beepOffInterval = offInterval;
	}
}

/*
 ������
 ���أ�
 ���ܣ�������ֹͣ����
*/
void beepStopBuzz(void)
{
	BEEP_OFF();
	
	buzzCounter = 0;
	buzzStatus = 0;
}


/*
 ������
 ���أ�
 ���ܣ�����������
*/
void beepTask(void)
{
	uint16_t curTick = getTick();
	if(buzzCounter > 0)
	{
		if((1 == buzzStatus) && (curTick - startTick >= beepOnInterval))
		{			
			if(buzzCounter != 0xffff)
			{				
				buzzCounter--;
                BEEP_OFF();
			}
			startTick = curTick;
			buzzStatus = 0;			
		}
		else if((0 == buzzStatus) && (curTick - startTick >= beepOffInterval))
		{
			BEEP_ON();
			startTick = curTick;
			buzzStatus = 1;			
		}
	}
}

