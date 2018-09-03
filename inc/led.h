/*
* �ļ�˵����led����
*/
#include "HC89S003F4.h"
#include "typedef.h"

#define BRIGHTNESS  ((uint16_t)800)
#define STEPS       ((uint8_t)20)   // �ж�50ms��20��Ϊ1s

typedef enum{
    Blue = 0,
    Green,
    Yellow,
    Red,
    Unknow
}Color_t;

typedef struct{  
    uint16_t currentRed;
    uint16_t currentGreen;
    uint16_t currentBlue;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    //uint16_t period;    // ������Ƶ�ʣ���λms
    uint16_t ccp;       // ����led��pwm���ڱȽ�ֵ
    uint8_t colorChanged;
    uint8_t redStep;
    uint8_t greenStep;
    uint8_t blueStep;
    Color_t color;
}LED_t;

extern LED_t led;




// end of file