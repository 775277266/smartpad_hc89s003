/*
* �ļ�˵����hc89s003f4ϵͳʱ������
*/
#include "HC89S003F4.h"
#include "hc89s003_clk.h"

/*
* ���룺div1-HRC Fosc��Ƶϵ��������ʱ�ӷ���ϵ��)��div2-HRC Fcpu��Ƶϵ��
* �����
* ���ܣ�����HRC��Ϊϵͳ��ʱ��HRCΪ32M��ͨ��div1(8/4/2/1)��ƵΪFosc���پ���div2(1~255)��ƵΪFcpu
*/
void HRCFreqConfig(uint8_t div1, uint8_t div2)
{
	// ʹ��HRC
	ENABLE_HSRC();
	
	// �ж�HRC�Ƿ�׼����
	while(IS_HSRC_READY() == 0);
	
	// ѡ��HRCΪоƬʱ��Դ
	CLK_SELECT(CLK_SEL_HSRC);
	
	// �ж�ʱ��Դ״̬
	while(CLK_STATUS != CLK_STA_HSRC);
	
	// �ر��ⲿ����
	DISABLE_XTAL();
	
	// ����Fosc��Ƶ
	SET_HSRC_DIV(div1);
	
	// ����Fcpu��Ƶ
	SET_CLK_DIV(div2);
    
    // flash��д����
    SET_FREQ_CLK(CPU_FREQ_4M);
}