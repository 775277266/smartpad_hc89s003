/*
* �ļ�˵�����ṩ�����еĳ�������ʱ��
*/
#include "HC89S003F4.h"
#include "hc89s003_timer3.h"
#include "hc89s003_adc.h"
#include "hc89s003_delay.h"
#include "board.h"
#include "tick.h"
#include "led.h"


static volatile uint16_t tick = 0;

/*
* ���룺
* �����
* ���ܣ����ص�ǰ����ʱ��
*/
uint16_t getTick(void)
{
    return tick;
}


/*
* ���룺
* �����
* ���ܣ�timer3�����жϣ��ж�����50ms
*/
void timer3Isr(void) interrupt T3_VECTOR
{
	tick++;
	
	switchAdcChannel();
    
    // ������Ч��
    if(led.colorChanged == 2)
    {
        // ���ȵݼ�
        led.colorChanged = 1;
        if(led.currentRed >= led.redStep)
        {
            led.currentRed -= led.redStep;
            led.colorChanged = 2;
        }
        else
        {
            led.currentRed = 0;
        }
        
        if(led.currentGreen >= led.greenStep)
        {
            led.currentGreen -= led.greenStep;
            led.colorChanged = 2;
        }
        else
        {
            led.currentGreen = 0;
        }
        
        if(led.currentBlue >= led.blueStep)
        {
            led.currentBlue -= led.blueStep;
            led.colorChanged = 2;
        }
        else
        {
            led.currentBlue = 0;
        }
        
        setPwm2Duty(led.currentRed);
        setPwm0Duty(led.currentGreen);
        setPwm1Duty(led.currentBlue);
    }
    else if(led.colorChanged == 1)
    {
        // ���ȵ���
        led.colorChanged = 0;
        if(led.red - led.currentRed <= led.redStep)
        {
            led.currentRed += led.redStep;
            led.colorChanged = 1;
        }
        else
        {
            led.currentRed = led.red;
        }
        
        if(led.green - led.currentGreen <= led.greenStep)
        {
            led.currentGreen += led.greenStep;
            led.colorChanged = 1;
        }
        else
        {
            led.currentGreen = led.green;
        }
        
        if(led.blue - led.currentBlue <= led.blueStep)
        {
            led.currentBlue += led.blueStep;
            led.colorChanged = 1;
        }
        else
        {
            led.currentBlue = led.blue;
        }
        
        setPwm2Duty(led.currentRed);
        setPwm0Duty(led.currentGreen);
        setPwm1Duty(led.currentBlue);
    }
}



// end of file
