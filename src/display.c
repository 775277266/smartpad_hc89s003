/*
 ����޸ģ�2018-05-06 by syf
 ��������������ǰ�������񣬴�����ʾ��Ӧ����ʾ����
*/
#include "display.h"
#include "typedef.h"
#include "display_driver.h"
#include "tick.h"
#include "common.h"
#include "key.h"
#include "board.h"
#include "alarm.h"


extern RunState_t runState;

/*
 ������
 ���أ�
 ���ܣ���ʾ�¶�ֵ
*/
static void displayTemperature(int16_t t)
{
    uint8_t b = 0;
    uint8_t s = 0;
    
    if(t < 0)
    {
        t = -t;
        if(t % 10 >= 5)
        {
            t = t / 10 + 1;
        }
        else
        {
            t = t / 10;
        }
        
        displayNum(1, '-');
        
        // ʵ��С��-99��
        if(t > 100)
        {
            displayNum(2, '9');
            displayNum(3, '9');
        }
        else
        {
            t = -t;
            s = t / 10;
            displayNum(2, s + '0');
            b = t % 10;
            displayNum(3, b + '0');
        }
    }
    else
    {
        if(t % 10 >= 5)
        {
            t = t / 10 + 1;
        }
        else
        {
            t = t / 10;
        }
        
        // ʵ���¶ȴ���999��
        if(t > 999)
        {
            displayNum(1, '9');
            displayNum(2, '9');
            displayNum(3, '9');
        }
        else
        {
            // �¶ȴ���100����ʾ��λ��
            if(t >= 100)        
            {
                b = t / 100;
                displayNum(1, b + '0');
            }
            
            // �¶ȴ���10����ʾʮλ��
            if(t >= 10)
            {
                s = (t - b * 100) / 10;
                displayNum(2, s + '0');
            }
            
            displayNum(3, t % 10 + '0');
        }
    }  
    
    // ��ʾ���϶ȷ���
    displayIcon(IconCelsius, True);
}

/*
 ������
 ���أ�
 ���ܣ���ʾ������
*/
static void displayAlarmMinute(uint8_t min)
{  
    // ����������10��ʾʮλ��
    if(min >= 10)
    {
        displayNum(2, min / 10 + '0');
    }
    
    displayNum(3, min % 10 + '0');
}

/*
 ������
 ���أ�
 ���ܣ���������״̬����ʾ��Ӧ������
*/
void displayTask(void)
{
	static uint16_t displayTick = 0;
	static uint8_t flashFlag = 0;
	
	uint16_t curTick = getTick();
    uint8_t batteryVoltageLv = getBatteryVoltageLv();
	
    // 100msˢ��һ��
	if(curTick - displayTick >= ELAPSED_TIME_100_MS)
	{
		displayTick = curTick;
		
		if(curTick % 10 >= ELAPSED_TIME_500_MS)
		{
			flashFlag = 1;
		}
		else
		{
			flashFlag = 0;
		}
		
		// ������ڵ���ʾ���ݣ���������Դ�
		clearDisplayRam();				
		
		// ��ʾ���״̬
		displayIcon(batteryVoltageLv, True);
        if(batteryVoltageLv == ICON_BAT_LV0)
        {
            if(flashFlag)
            {
                displayIcon(batteryVoltageLv, False);
            }
        }
	
		switch(runState)
		{
			case FreeRunState:
                // ��ʾ�¶�
      displayTemperature(getTemperature());
				break;
			case SettingAlarmState:
				displayIcon(IconClock, True);
                displayIcon(IconMin, True);
				displayAlarmMinute(getAlarmMinutes());
				break;
			case AlarmCountDownState:
				if(flashFlag)
				{
					displayIcon(IconClock, False);
				}
                else
                {
                    displayIcon(IconClock, True);
                }
                displayIcon(IconMin, True);
				displayAlarmMinute(getAlarmMinutes());			
				break;
			case ShutDownState:
                // �ػ���ʾOFF
				clearDisplayRam();
				displayNum(1, '0');
                displayNum(2, 'F');
                displayNum(3, 'F');
				updataDisplayRam();
				break;
			default:
				break;
		}		
		updataDisplayRam();
	}
}


// end of file
