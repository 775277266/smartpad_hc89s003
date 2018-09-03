/*
 ����޸ģ�2018-05-06 by syf
 �������������Ӳ���
*/

#include "alarm.h"
#include "common.h"
#include "tick.h"

static uint8_t alarmMinutes = 0;		// ��ʱ�ķ�����0-99
static uint16_t alarmTick = 0;			// ���ӵ���ʱ����ʱ��ʱ���

/*
 ������
 ���أ�
 ���ܣ���ȡ���ӵ���ʱʣ�������
*/
uint8_t getAlarmMinutes(void)
{
	return alarmMinutes;
}

/*
 ������
 ���أ�
 ���ܣ�����趨������
*/
void clearAlarmMinutes(void)
{
	alarmMinutes = 0;
}

/*
 ������
 ���أ�
 ���ܣ����ӷ���������1
*/
void incAlarmMinutes(void)
{
	alarmTick = getTick();
	
	if(alarmMinutes < MAX_ALARM_MINUTES)
	{
		alarmMinutes++;
	}
	else
	{
		alarmMinutes = 0;
	}
}

/*
 ������
 ���أ�
 ���ܣ����ӵ���ʱ����
*/
void alarmCountDown(void)
{
	uint16_t curTick = getTick();
	
	if(curTick - alarmTick >= ELAPSED_TIME_1_MINUTE)
	{
		alarmTick = curTick;
		
		if(alarmMinutes > 0)
		{
			alarmMinutes--;
		}
	}
}

// end of file
