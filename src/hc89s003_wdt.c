/*
* �ļ�˵����hc89s003f4���Ź�ģ��
*/
#include "HC89S003F4.h"
#include "hc89s003_wdt.h"
#include "hc89s003_delay.h"

static uint8_t wdtCCR = 0;

/*
* ���룺div-wdt����ʱ�ӷ�Ƶ��ccr-wdt����Ƚ�ֵ
* �����
* ���ܣ�wdt��ʱ��ԴΪ44k�ڲ�����RC��wdt�����ʱ����㷽ʽΪdiv * ccr / 44000
*/
void WDTConfig(uint8_t div, uint8_t ccr)
{
    DISABLE_WDT();
    delay_ms(1);
    
    wdtCCR = ccr;
    SET_WDT_DIV(div);
    //SET_WDT_CCR(ccr);
    
    //ENABLE_WDT();
}

/*
* ���룺
* �����
* ���ܣ�����wdt
*/
void startWdt(void)
{
    ENABLE_WDT();
    SET_WDT_CCR(wdtCCR);
}


// end of file
