/*
* �ļ�˵����������Ŀhc89s003f4ϵͳ���������õĺ���
*/
#include "mcu_config.h"
#include "HC89S003F4.h"

/*
* ���룺
* �����
* ���ܣ�ѡ��HC89S003��ʱ��ԴΪHRC��Fosc = 4M(8��Ƶ)��Fcpu = 4M(1��Ƶ��
*/
void initSystemClk()
{
	HRCFreqConfig(RC32M_DIV_8, 1);
	SET_FREQ_CLK(CPU_FREQ_4M);
}