/*
 ����޸ģ�2018-05-06 by syf
 �������������Ӳ���
*/

#ifndef ALARM_H
#define ALARM_H

#include "typedef.h" 

#define MAX_ALARM_MINUTES	((uint8_t)99)

uint8_t getAlarmMinutes(void);
void clearAlarmMinutes(void);
void incAlarmMinutes(void);
void alarmCountDown(void);



#endif	// ALARM_H
