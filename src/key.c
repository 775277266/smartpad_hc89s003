/*
 ����޸ģ�2018-05-06 by syf
 ��������������ɨ�裬��״̬�л�
*/
#include "typedef.h"
#include "key.h"
#include "alarm.h"
#include "beep.h"
#include "board.h"
#include "common.h"
#include "tick.h"
#include "HC89S003F4.h"

#define KEY_STATE (P2_7)

static uint8_t keyPressFlag = 0;	// ����״̬
static uint8_t lastKeyValue = KEY_NONE;
static uint16_t keyPressedTick = 0;	// ��⵽����������ʱ��ʱ��
static uint16_t keyTaskTick = 0;
static uint16_t lowTemperatureTick = 0;
static uint8_t lowTemperatureFlag = 0;

RunState_t runState = FreeRunState;
static uint16_t settingAlarmTick = 0;	// �ж��˳������趨���������ӵ���ʱ��ʱ���


/*
 ������
 ���أ���ǰ����ֵ
 ���ܣ�����ɨ�����
*/	
static uint8_t keyScan(void)
{
	uint16_t curTick = getTick();
	uint16_t incTick = 0;
	uint8_t keyValue = KEY_NONE;
	
	if(0 == KEY_STATE)	// ����Ϊ����״̬
	{
		if(0 == keyPressFlag)
		{
			// �����ձ�����
			keyPressFlag = 1;
			keyPressedTick = curTick;
		}
		else
		{
			// �������ж�
			incTick = curTick - keyPressedTick;
			if(incTick >= KEY_DELAY_2S)
			{
				keyValue = KEY_ALARM;
			}
			
			if(incTick >= KEY_DELAY_4S)
			{
				keyValue = KEY_POWER_OFF;
			}
		}
	}
	else	// ����Ϊ����״̬
	{
		if(1 == keyPressFlag)
		{
			incTick = curTick - keyPressedTick;
			if(incTick < KEY_DELAY_2S)
			{
				keyValue = KEY_SHORT_PRESS;
			}
		}
		
		keyPressFlag = 0;
	}
	
	// �����ظ��ĳ�����ֵ
	switch(keyValue)
	{
		case KEY_ALARM:
			if(lastKeyValue == KEY_ALARM)
			{
				keyValue = KEY_NONE;
			}
			else
			{
				lastKeyValue = keyValue;
			}
			break;
		case KEY_POWER_OFF:
			if(lastKeyValue == KEY_POWER_OFF)
			{
				keyValue = KEY_NONE;
			}
			else
			{
				lastKeyValue = keyValue;
			}
			break;
		case KEY_SHORT_PRESS:
		case KEY_NONE:
		default:
			lastKeyValue = keyValue;
			break;
	}
	
	return keyValue;
}

/*
 ������
 ���أ�
 ���ܣ����ݰ�������������FreeRunStateʱ�Ĺ���
*/	
static void processFreeRunState(uint8_t key)
{
	uint16_t curTick = getTick();
	
	if(getTemperature() <= MIN_TEMPERATURE)
	{
		if(0 == lowTemperatureFlag)
		{
			lowTemperatureTick = curTick;
			lowTemperatureFlag = 1;
		}
	}
	else
	{
		lowTemperatureFlag = 0;
	}
	
	switch(key)
	{
		case KEY_POWER_OFF:
			runState = ShutDownState;
			//powerOff();
			break;
		case KEY_ALARM:
			// ���������趨״̬����������һ��
			clearAlarmMinutes();
			beepStartBuzz(1, ELAPSED_TIME_200_MS, ELAPSED_TIME_500_MS);
			settingAlarmTick = curTick;
			runState = SettingAlarmState;
			break;
		case KEY_SHORT_PRESS:
			break;
		case KEY_NONE:
			if((1 == lowTemperatureFlag) && (curTick - lowTemperatureTick >= ELAPSED_TIME_1_MINUTE))
			{
				// �¶�С��50���϶ȣ�����ʱ�䳬��1����
				runState = ShutDownState;
			}
		default:
			break;
	}
}

/*
 ������
 ���أ�
 ���ܣ����ݰ�������������SetttingAlarmStateʱ�Ĺ���
*/
static void processSettingAlarmState(uint8_t key)
{
	uint16_t curTick = getTick();
	switch(key)
	{
		case KEY_POWER_OFF:
			runState = ShutDownState;
			//powerOff();
			break;
		case KEY_ALARM:
			// ������ӣ���������һ��
			clearAlarmMinutes();
			beepStartBuzz(1, ELAPSED_TIME_300_MS, ELAPSED_TIME_500_MS);
			runState = FreeRunState;
			break;
		case KEY_SHORT_PRESS:
			// �����趨��������1
			incAlarmMinutes();
			settingAlarmTick = getTick();
			break;
		case KEY_NONE:
			// 3�����ް������˳�SettingAlarmState����������һ��
			if(curTick - settingAlarmTick >= ELAPSED_TIME_5_SECOND)
			{
				if(getAlarmMinutes() > 0)
				{
					// �趨�����Ӵ���1���ӣ�����AlarmCountDown״̬
					runState = AlarmCountDownState;					
				}
				else
				{
					// �趨������Ϊ0������FreeRunState
					runState = FreeRunState;
				}
				
				beepStartBuzz(1, ELAPSED_TIME_300_MS, ELAPSED_TIME_500_MS);
			}
			break;
		default:
				break;
	}
}

/*
 ������
 ���أ�
 ���ܣ����ݰ�������������AlarmCountDownʱ�Ĺ���
*/
static void processAlarmCountDownState(uint8_t key)
{
	switch(key)
	{
		case KEY_POWER_OFF:
			runState = ShutDownState;
			//powerOff();
			break;
		case KEY_ALARM:
			// ������ӣ���������һ��
			clearAlarmMinutes();
			beepStartBuzz(1, ELAPSED_TIME_300_MS, ELAPSED_TIME_500_MS);
			runState = FreeRunState;
			break;
		case KEY_SHORT_PRESS:
		case KEY_NONE:
		default:
				break;
	}
	
	if(AlarmCountDownState == runState)
	{
		alarmCountDown();
		
		// ���ӵ���ʱ��������������3��
		if(0 == getAlarmMinutes())
		{
			runState = FreeRunState;
			beepStartBuzz(3, ELAPSED_TIME_300_MS, ELAPSED_TIME_700_MS);
		}
	}
}


/*
 ������
 ���أ�
 ���ܣ��������������豸������״̬���ɰ����ı�ģ���˰�״̬�л�Ҳ�����������
*/	
void keyTask(void)
{
	uint16_t curTick = getTick();
	uint8_t key = KEY_NONE;
	
	// system�仯ʱ�ż�ⰴ��
	if(keyTaskTick != curTick)
	{
		keyTaskTick = curTick;
		
		key = keyScan();

// ���ڲ��԰��������Ƿ�����		
//		switch(key)
//		{
//			case KEY_SHORT_PRESS:
//				dbg_printf("key short!\r\n");
//				break;
//			case KEY_ALARM:
//				dbg_printf("key alarm!\r\n");
//				break;
//			case KEY_POWER_OFF:
//				dbg_printf("key power off!\r\n");
//				break;
//			default:break;
//		}
		
		switch(runState)
		{
			case FreeRunState:
				processFreeRunState(key);
				break;
			case SettingAlarmState:
				processSettingAlarmState(key);
				break;
			case AlarmCountDownState:
				processAlarmCountDownState(key);
				break;
			case ShutDownState:
				break;
			default:
				break;
		}
	}
}

/*
 ������
 ���أ�
 ���ܣ���Դ������������
*/	
void powerTask(void)
{
	if(ShutDownState == runState)
	{
		powerOff();
	}
}




// end of file
