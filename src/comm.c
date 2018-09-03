/*
* �ļ�˵����ͨ�������ݴ洢
*/


#include "HC89S003F4.h"
#include "hc89s003_timer4.h"
#include "hc89s003_uart.h"
#include "hc89s003_int.h"
#include "hc89s003_flash.h"
#include "board.h"
#include "tick.h"
#include "comm.h"

#define FLASH_ADDR		(0x3f00)
#define FLASH_ADDR_BAK	(0x3f80)	// ���һ������
#define RX_BUF_LENGTH	(12)
static XDATA uint8_t rxBuf[RX_BUF_LENGTH] = 0;
static uint8_t rxPtr = 0;

extern XDATA uint16_t warningNum;

/*
* ���룺
* �����
* ���ܣ�uart1�����ж�
*/
void UART1_ISR(void) interrupt UART1_VECTOR
{
	//�жϽ����жϱ�־λ
	if(SCON & SCON_RI)						
	{
		//ת��8λ���ڽ�������
		if(rxPtr < RX_BUF_LENGTH)
		{
			rxBuf[rxPtr++] = SBUF;
		}
		
		//��������жϱ�־λ
		SCON &=~SCON_RI;					
	}									
}



/*
* ���룺dat-�����׵�ַ��length-���ݳ���
* �����У���
* ���ܣ�У��ͼ���
*/
static uint8_t checkSum(uint8_t *dat, uint8_t length)
{
	uint8_t sum = 0;
	uint8_t i = 0;
	for(i = 0; i < length; i++)
	{
		sum += dat[i];
	}
	
	return sum;
}

/*
* ���룺
* �����
* ���ܣ���ȡ�ȵ�ż���¶ȼ���ֱ�߲���
*/
void readTempeartureParam(TemperatureParam_t *param)
{
	uint8_t tmp[5];
	Flash_ReadArr(FLASH_ADDR, 5, tmp);
	
	if(tmp[4] != checkSum(tmp, 4))
	{
		Flash_ReadArr(FLASH_ADDR_BAK, 5, tmp);
		
		if(tmp[4] != checkSum(tmp, 4))
		{
			return;
		}
	}
	
	param->k = ((uint16_t)tmp[0]) << 8;
	param->k += tmp[1];
	
	param->b = ((uint16_t)tmp[2]) << 8;
	param->b += tmp[3];
}

#define RESET_RX_BUF()	do{rxPtr = 0;}while(0)
#define START_CODE ((uint8_t)0x2f)

/*
* ���룺
* �����
* ���ܣ�ͨ�����ڷ�������
*/
static void uartSendData(uint8_t *d, uint8_t len)
{
    uint8_t i = 0;
    DISABLE_UART1_INT();
    
    for(i= 0; i < len; i++)
	{
		SBUF = d[i];        //����8λ��������
		while(!(SCON & SCON_TI));
		SCON &= ~SCON_TI;   //��������жϱ�־λ
	}
    SCON &= ~SCON_RI;
    ENABLE_UART1_INT();
}

/*
* ���룺
* �����
* ���ܣ�����Э�����
* һ֡���ݹ�8�ֽ�
* byte1-0x2f��֡��ʼ��
* byte2-������
* byte3~byte7-���ݶ�
* byte8-У���
*/
static void rxDataProcess(void)
{
	uint8_t ptr = rxPtr;
	uint8_t i;
	if(ptr >= 8)
	{
		for(i = 0; i < ptr; i++)
		{
			if(rxBuf[i] == START_CODE)
			{
				break;
			}
		}
		
		if(ptr - i >= 8)
		{
			// У��ɹ����յ���������֡
			if(checkSum(rxBuf + i + 1, 6) == rxBuf[i + 7])
			{
				// ��������У���
				rxBuf[i + 6] = checkSum(rxBuf + i + 2, 4);
				
                
                if(rxBuf[i + 1] == WRTIE_PARAMETERS)
                {
                    // �洢������
                    Flash_EraseBlock(FLASH_ADDR);
                    Flash_WriteArr(FLASH_ADDR, 5, rxBuf + i + 2);
                    
                    // �洢������
                    Flash_EraseBlock(FLASH_ADDR_BAK);
                    Flash_WriteArr(FLASH_ADDR_BAK, 5, rxBuf + i + 2);
                    
                    // ��λ���ջ���ָ��
                    RESET_RX_BUF();
                }
                else if(rxBuf[i + 1] == READ_WARNING_NUM)
                {
                    uartSendData((uint8_t*)&warningNum, 2);
                }				
			}
		}
		else if(i + 8 >= RX_BUF_LENGTH)
		{
			RESET_RX_BUF();
		}
	}
	
	if(ptr >= RX_BUF_LENGTH)
	{
		RESET_RX_BUF();
	}
}

/*
* ���룺
* �����
* ���ܣ�ͨ������
*/
void commTask(void)
{
	static uint16_t oldTick = 0;
	uint16_t newTick = getTick();
	
	if(oldTick != newTick)
	{
		oldTick = newTick;
		rxDataProcess();
	}
}






// end of file
