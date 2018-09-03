/*
* �ļ�˵����hc89s003f4 GPIOģ��
*/
#include "HC89S003F4.h"
#include "hc89s003_gpio.h"



/*
* ���룺port-�˿ڣ� pins-���ţ� mode-ģʽ
* �����
* ���ܣ�����gpio�Ĺ���ģʽ
*/
void gpioConfig(uint8_t port, uint8_t pins, GPIOMode_t mode)   
{
    uint8_t temp = 0;
    switch(port)
    {
        case GPIO_P0:
            temp = P0M0;
            if(pins & 0x01)
            {
                temp &= 0xf0;
                temp |= (uint8_t)mode;
            }
            
            if(pins & 0x02)
            {
                temp &= 0x0f;
                temp |= (uint8_t)mode << 4;
            }
            P0M0 = temp;
            
            temp = P0M1;
            if(pins & 0x04)
            {
                temp &= 0xf0;
                temp |= (uint8_t)mode;
            }
            
            if(pins & 0x08)
            {
                temp &= 0x0f;
                temp |= (uint8_t)mode << 4;
            }
            P0M1 = temp;
            
            temp = P0M2;
            if(pins & 0x10)
            {
                temp &= 0xf0;
                temp |= (uint8_t)mode;
            }
            
            if(pins & 0x20)
            {
                temp &= 0x0f;
                temp |= (uint8_t)mode << 4;
            }
            P0M2 = temp;
            
            temp = P0M3;
            if(pins & 0x40)
            {
                temp &= 0xf0;
                temp |= (uint8_t)mode;
            }
            
            if(pins & 0x80)
            {
                temp &= 0x0f;
                temp |= (uint8_t)mode << 4;
            }
            P0M3 = temp;           
            break;
        case GPIO_P1:
            temp = P1M0;
            if(pins & 0x01)
            {
                temp &= 0xf0;
                temp |= (uint8_t)mode;
            }
            
            if(pins & 0x02)
            {
                temp &= 0x0f;
                temp |= (uint8_t)mode << 4;
            }
            P1M0 = temp;
            break;
        case GPIO_P2:
            temp = P2M0;
            if(pins & 0x01)
            {
                temp &= 0xf0;
                temp |= (uint8_t)mode;
            }
            
            if(pins & 0x02)
            {
                temp &= 0x0f;
                temp |= (uint8_t)mode << 4;
            }
            P2M0 = temp;
            
            temp = P2M1;
            if(pins & 0x04)
            {
                temp &= 0xf0;
                temp |= (uint8_t)mode;
            }
            
            if(pins & 0x08)
            {
                temp &= 0x0f;
                temp |= (uint8_t)mode << 4;
            }
            P2M1 = temp;
            
            temp = P2M2;
            if(pins & 0x10)
            {
                temp &= 0xf0;
                temp |= (uint8_t)mode;
            }
            
            if(pins & 0x20)
            {
                temp &= 0x0f;
                temp |= (uint8_t)mode << 4;
            }
            P2M2 = temp;
            
            temp = P2M3;
            if(pins & 0x40)
            {
                temp &= 0xf0;
                temp |= (uint8_t)mode;
            }
            
            if(pins & 0x80)
            {
                temp &= 0x0f;
                temp |= (uint8_t)mode << 4;
            }
            P2M3 = temp;
            break;
    }
}







// end of file
