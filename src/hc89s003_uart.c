/*
* �ļ�˵����hc89s003f4���ڲ���
*/
#include "HC89S003F4.h"
#include "hc89s003_timer4.h"
#include "hc89s003_uart.h"
#include "hc89s003_int.h"
#include "tick.h"



/*
* ���룺
* �����
* ���ܣ�uartģʽ1���첽˫����������9600
*/
void uart1Config()
{
	// uart mode1 ��timer4��Ϊ�����ʷ�����
	// FoscΪʱ��Դ������Ƶ
	T4CON = T4PS_1 + T4M_MODE1 + T4CLK_FOSC;
	
	//�����ʼ���
	//������ = 1/16 * (T4ʱ��ԴƵ�� / ��ʱ��4Ԥ��Ƶ��) / (65536 - T4)
	//       = 1/16 * ((4000000 / 1) / T4)

	//������9600
	//���Ƴ�ֵ = (65536 - ((T4ʱ��ԴƵ�� / ��ʱ��4Ԥ��Ƶ��) * (1 / 16)) / ������)
	//		   = (65536 - (4000000 * (1 / 16) / 9600))
	//		   = (65536 - 26.04167)
	//         = 65510(FFE6)
	TH4 = 0xFF;
	TL4 = 0xE6;
	ENABLE_T4();
	
	// ѡ��uart mode1�� ʹ���жϽ���
	SCON2 = SCON2_MODE1;
	SCON = SCON_REN;
	
	// ʹ�ܴ����ж�
	ENABLE_UART1_INT();
}



// end of file
