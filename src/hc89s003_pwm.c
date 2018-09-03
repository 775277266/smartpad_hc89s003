/*
* �ļ�˵����hc89s003f4 12λpwmģ�飬����pwm0��1,2��������3·���
*/
#include "HC89S003F4.h"
#include "hc89s003_pwm.h"
#include "hc89s003_gpio.h"
#include "led.h"



/*
* ���룺d-pwmռ�ձ�
* �����
* ���ܣ�����pwm1ģ���ռ�ձ�
*/
void setPwm0Duty(uint16_t p)
{
    uint16_t tmp;
    
    if(p > 1000)
    {
        p = 1000;
    }
    
    tmp = (uint32_t)led.ccp * p / 1000;
    
    PWM0DH = (uint8_t)(tmp >> 8) & 0x0f;
    PWM0DL = (uint8_t)tmp;
}

/*
* ���룺d-pwmռ�ձ�
* �����
* ���ܣ�����pwm1ģ���ռ�ձ�
*/
void setPwm1Duty(uint16_t p)
{
    uint16_t tmp;
    
    if(p > 1000)
    {
        p = 1000;
    }
    
    tmp = (uint32_t)led.ccp * p / 1000;
    
    PWM1DH = (uint8_t)(tmp >> 8) & 0x0f;
    PWM1DL = (uint8_t)tmp;
}

/*
* ���룺d-pwmռ�ձ�
* �����
* ���ܣ�����pwm2ģ���ռ�ձ�
*/
void setPwm2Duty(uint16_t p)
{
    uint16_t tmp;
    
    if(p > 1000)
    {
        p = 1000;
    }
    tmp = (uint32_t)led.ccp * p / 1000;
    
    PWM2DH = (uint8_t)(tmp >> 8) & 0x0f;
    PWM2DL = (uint8_t)tmp;
}

/*
* ���룺
* �����
* ���ܣ�pwm0ģ������
*/
void pwm0Config(void)
{
    DISABLE_PWM(PWM0EN);
    
    PWM0C = PWM_OUT_LL | PWM_CLK_FOSC_1;
    PWM0EN |= PWM_OUT_EN;
    
    // ʱ��4M�� ����1k������ֵΪ4000 - 1
    PWM0PH = 0x0F;
    PWM0PL = 0x9f;
    
    setPwm0Duty(50);
    
    ENABLE_PWM(PWM0EN);
}

/*
* ���룺
* �����
* ���ܣ�pwm1ģ������
*/
void pwm1Config(void)
{
    DISABLE_PWM(PWM1EN);
    
    PWM1C = PWM_OUT_LL | PWM_CLK_FOSC_1;
    PWM1EN |= PWM_OUT_EN;
    
    // ʱ��4M�� ����1k������ֵΪ4000 - 1
    PWM1PH = 0x0F;
    PWM1PL = 0x9f;
    
    setPwm1Duty(50);
    
    ENABLE_PWM(PWM1EN);
}

/*
* ���룺
* �����
* ���ܣ�pwm2ģ������
*/
void pwm2Config(void)
{
    DISABLE_PWM(PWM2EN);
    
    PWM2C = PWM_OUT_LL | PWM_CLK_FOSC_1;
    PWM2EN |= PWM_OUT_EN;
    
    // ʱ��4M�� ����1k������ֵΪ4000 - 1
    PWM2PH = 0x0F;
    PWM2PL = 0x9f;
    
    setPwm2Duty(50);
    
    ENABLE_PWM(PWM2EN);
}

/*
* ���룺
* �����
* ���ܣ�pwm3ģ������
*/
void pwm3Config(void)
{
    //DISABLE_PWM3();
    
    PWM3C = PWM3_OUT_EN | PWM3_OUT_H | PWM3_CLK_FOSC_16;
    
    // ʱ��4M / 16 = 250K�� ���� 1/2.8K�� ����ֵ = 250 / 2.8 = 89
    PWM3P = 89;
    // 50%ռ�ձ�
    PWM3D = 44;
}



// end of file
