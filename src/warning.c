/*
* �ļ�˵�������±�����RGB����ɫָʾ
*/

#include "typedef.h"
#include "warning.h"
#include "board.h"
#include "common.h"
#include "tick.h"
#include "beep.h"
#include "hc89s003_flash.h"
#include "led.h"

// ��2���ֽ�ȥ��¼����300�ȵĴ�������ΪFLASH������ĳ�ʼֵΪ0x00��Ϊ���ָ������Ƿ���ʹ�ã�д��Ĵ����ķ�ΧΪ0x0001~0xffff
// Ϊ����flash�Ĳ�д������ֻ��һ��������д��ʱ�Ų����������´������׵�ַ��ʼд��������ֵΪ0xffff��ʾ������δʹ��
#define WARNNING_NUM_ADDR   ((uint16_t)0x3e80)
#define BLOCK_SIZE          ((uint8_t)0x80)

XDATA uint16_t warningNum = 0;

/*
* ���룺
* �����
* ���ܣ����±�����������1
*/
static void warningNumInc(void)
{
    if(warningNum < 0xffff)
    {
        warningNum++;
    }
}

/*
* ���룺
* �����
* ���ܣ�д����¸澯����
*/
void saveWarningNum(void)
{
    uint8_t n = 0;
    uint16_t temp = 0;
    
    // Ѱ��δд��ĵ�ַ
    while(n < BLOCK_SIZE)
    {
        Flash_ReadArr(n + WARNNING_NUM_ADDR, 2, (uint8_t*)&temp);
        
        if(temp == 0x0)
        {
            break;
        }
        else
        {
            n += 2;
        }
    }
    
    // �������������д��
    if(n == BLOCK_SIZE)
    {
        Flash_EraseBlock(WARNNING_NUM_ADDR);
        n = 0;
    }
    
    Flash_WriteArr(n + WARNNING_NUM_ADDR, 2, (uint8_t*)&warningNum);
}

/*
* ���룺
* �����
* ���ܣ���ȡ���¸澯����
*/
void readWarningNum(void)
{
    uint8_t n = 0;
    uint16_t temp = 0;
    
    // Ѱ�����һ��д��ֵ
    while(n < BLOCK_SIZE)
    {
        Flash_ReadArr(n + WARNNING_NUM_ADDR, 2, (uint8_t*)&temp);
        
        if(temp == 0x0)
        {
            break;
        }
        else
        {
            n += 2;
        }
    }
    
    if(n != 0)
    {
        n -= 2;
        Flash_ReadArr(n + WARNNING_NUM_ADDR, 2, (uint8_t*)&warningNum);
    }
    else
    {
        warningNum = 0;
    }
}

/*
* ���룺
* �����
* ���ܣ����±�����RGB����ɫ��������
*/
void warningTask(void)
{
    static uint16_t oldTick = 0;
    static uint8_t warningFlag = 0;
    uint16_t tick = getTick();
    int16_t t;
    
    if(tick - oldTick >= ELAPSED_TIME_1_SECOND)
    {
        oldTick = tick;
        
        t = getTemperature();
        
        // ����300�ȱ���������300-5�Ƚ�������
        if(t >= WARNING_TEMPERATURE)
        {
            if(warningFlag == 0)
            {
                // 0xffff��ʾ�������������
                beepStartBuzz(0xffff, ELAPSED_TIME_300_MS, ELAPSED_TIME_700_MS);
                warningNumInc();
                saveWarningNum();
                
                warningFlag = 1;
            }
        }
        else if (t <= WARNING_TEMPERATURE - 50)
        {
            if(warningFlag == 1)
            {
                beepStopBuzz();
                warningFlag = 0;
            }
        }
        
        if(t >= RED_TEMPERATURE)
        {
            if(led.color != Red)
            {
                setRGB(255, 0, 0, BRIGHTNESS);
                led.color = Red;
            }                
        }
        else if(t >= YELLOW_TEMPERATURE)
        {
            if(led.color != Yellow)
            {
                setRGB(255, 255, 0, BRIGHTNESS);
                led.color = Yellow;
            }
        }
        else if(t >= GREEN_TEMPERATURE)
        {
            if(led.color != Green)
            {
                setRGB(0, 255, 0, BRIGHTNESS);
                led.color = Green;
            }
        }
        else
        {
            if(led.color != Blue)
            {
                setRGB(0, 0, 255, BRIGHTNESS);
                led.color = Blue;
            }
        }
    }
}




// end of file
