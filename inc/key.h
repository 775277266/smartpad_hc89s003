#ifndef _KEY_H_
#define _KEY_H_

#include "common.h"


#define KEY_DELAY_2S		((uint16_t)(40))	
#define KEY_DELAY_4S		((uint16_t)(80))

#define KEY_NONE			(0x0)
#define KEY_SHORT_PRESS		(0x01)
#define KEY_ALARM			(0x02)
#define KEY_POWER_OFF		(0x03)

#define KEY (BIT6)
#define NO_KEY	0
#define SW1  	1

typedef enum{
	FreeRunState = 0,		// ֻ�����¶ȣ�����ʾ�Ĺ���״̬
	SettingAlarmState,		// ������������ʱ���״̬
	AlarmCountDownState,	// ���ӵ���ʱ��״̬
	ShutDownState     		// ׼���ػ�״̬
}RunState_t;


void keyTask(void);
void powerTask(void);

#endif
