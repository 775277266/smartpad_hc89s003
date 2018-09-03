/*
* �ļ�˵����hc89s003f4 timer3ģ�����
*/
#include "HC89S003F4.h"
#include "hc89s003_timer3.h"

/*
* ���룺v-���ؼ�����ֵ
* �����
* ���ܣ�����timer3�����ؼ�����
*/
void setTimer3ReloadCounter(uint16_t v)
{
    DISABLE_TIMER3();
    
    TH3 = (uint8_t)(v >> 8);
    TL3 = (uint8_t)v;
}

/*
* ���룺status-�ж�״̬ʹ�ܻ��ֹ��pri-�ж����ȼ�
* �����
* ���ܣ�����timer3�ж�
*/
void timer3InterruptConfig(FlagStatus_t status, IntPriority_t pri)
{
    uint8_t ip2 = 0;
    if(Set == status)
    {
        // ���жϱ�־λ
        T3CON &= 0x7f;       
        
        // �����ж����ȼ�
        if(pri >= IntPriority3)
        {
            pri = IntPriority0;
        }
        
        ip2 = IP2;
        ip2 &= 0xf3;
        ip2 |= ((uint8_t)pri) << 2;
        IP2 = ip2;
		
		// �����ж�ʹ��λ
        ENABLE_T3_INT();
        
    }
    else
    {
        // ���жϱ�־λ
        T3CON &= 0x7f;
        
        // �����ж�ʹ��λ
        DISABLE_T3_INT();
    }
}



/*
* ���룺
* �����
* ���ܣ�����timer3���������жϣ��ж�����Ϊ50ms
*/
void timer3Config(void)
{
    DISABLE_TIMER3();
    
    // ϵͳʱ����Ԥ��Ƶ���ã�ѡ��FoscΪϵͳʱ�ӣ�64��Ƶ
    T3CON |= T3_CLK_FOSC | T3_PS_64;
    
    // ����Ƶ��Ϊ4000000/64=62500
    // ����100ms���жϣ���װ��ֵΪ65536 - 3125 = 59286(0xF3CB)
    TH3 = 0xf3;
    TL3 = 0xcb;
    
    // ʹ���жϣ��ж����ȼ�Ϊ0
    timer3InterruptConfig(Set, IntPriority0);
    
    ENABLE_TIMER3();
}




// end of file
