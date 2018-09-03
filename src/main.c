/*
* �ļ�˵������Ŀ������
*/
#define	ALLOCATE_EXTERN
#include "HC89S003F4.h"
#include "mcu_config.h"
#include "board.h"
#include "comm.h"
#include "key.h"
#include "alarm.h"
#include "display.h"
#include "beep.h"
#include "warning.h"
#include "ht1621.h"
#include "intrins.h"
#include "stdlib.h"

//uint8_t arr[4] = {0};

void main(void)
{  
    // �رտ��Ź�

    DISABLE_WDT();
    
    // ��Դ����
    powerOn(); 
    
    initWdt();
	initGpio();
	initSystemClk();
	initAdc();
	initTimer();
    initUart();
	initLEDControl();
	initBeepControl();
    initLcdDriver();
    //initTemperatureMeasure();
    readWarningNum();
    
    //startWdt();
	ENABLE_INTERRUPT();
	
    while(1)
    {     
        // �忴�Ź�
        //CLEAR_WDT();
    
        // ����ɨ�裬 ����״̬�л�
		keyTask();
        
		// ��������״̬����ʾ��Ӧ����
		displayTask();

        // ��������
        warningTask();
		
		// ����������
		beepTask();
		
		// ����ͨ������
		commTask();
        
        // ��Դ������������
		powerTask();
    }
}

// end of file
