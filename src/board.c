/*
* �ļ�˵����
*/

#include "board.h"
#include "ht1621.h"
#include "led.h"

#define MAX_PTR	((uint8_t)12)
static uint8_t ptr = 0;
static AdcChannel_t ch = BatteryVoltage;
static XDATA uint16_t adcResultBuffer[MAX_PTR] = {0};
static XDATA uint8_t batteryVoltageLv = IconBatLv3;	// 4.0V
static XDATA int16_t temperature = 0;	// 0��
static TemperatureParam_t param = {2643, 3878};

/*
* ���룺
* �����
* ���ܣ���ʼ�����Ź�ģ��
*/
void initWdt(void)
{
    // ��λ����Ϊ3�� (255 + 1) * 512 / 44000 = 2.9789
    WDTConfig(WDT_DIV_512, 255);
}

/*
* ���룺
* �����
* ���ܣ�������Ŀ��·�壬��ʼ��MCU����
*/
void initGpio(void)
{  
    // ����K1-P2.7
    gpioConfig(GPIO_P2, GPIO_PIN_7, Input_PullUp_NonSchmitt);
	
	// Һ������P1.1
	gpioConfig(GPIO_P1, GPIO_PIN_1, Output_PP);
	P1_1 = 1;
}

/*
* ���룺
* �����
* ���ܣ�ADCģ���ʼ��
*/
void initAdc(void)
{
    // P0.2-�¶ȼ��AD��P0.1��ص�ѹ���AD
	gpioConfig(GPIO_P0, GPIO_PIN_1 | GPIO_PIN_2, Analog);
    
	// 2.7V�ڲ��ο���ѹ��500kת��ʱ��
	adcConfig();
	XCH_SEL((uint8_t)BatteryVoltage);
	delay_us(10);
	START_CONVERT();
	
}

/*
* ���룺
* �����
* ���ܣ���ʱ����ʼ��
*/
void initTimer(void)
{
	timer3Config();
}

/*
* ���룺
* �����
* ���ܣ�RGB LED���Ƴ�ʼ��
*/
void initLEDControl(void)
{
    // PWM_LEDR-PWM0-P0.6��PWM_LEDG-PWM1-P0.7��PWM_LEDB-PWM2-P2.0
	gpioConfig(GPIO_P0, GPIO_PIN_6 , Output_PP);
    gpioConfig(GPIO_P0, GPIO_PIN_7 , Output_PP);
	GPIO_PIN_MAP(PWM0_MAP, GPIO_P0, 6);
    GPIO_PIN_MAP(PWM1_MAP, GPIO_P0, 7);
    gpioConfig(GPIO_P2, GPIO_PIN_0, Output_PP);
    GPIO_PIN_MAP(PWM2_MAP, GPIO_P2, 0);
    
    // ʱ��4M�� ����1k������ֵΪ4000 - 1
    led.ccp = 3999; 
    
	pwm0Config();
	pwm1Config();
	pwm2Config();	
    
    setRGB(0, 0, 255, BRIGHTNESS);
    led.color = Blue;
}

/*
* ���룺
* �����
* ���ܣ�pwm�����ʼ��
*/
void initBeepControl(void)
{
    // PWM_BEEP-PWM3-P0.0
    gpioConfig(GPIO_P0, GPIO_PIN_0, Output_PP);
    GPIO_PIN_MAP(PWM3_MAP, GPIO_P0, 0);
	P0_0 = 0;
    
    pwm3Config();
}

/*
* ���룺
* �����
* ���ܣ�uart�����ʼ��
*/
void initUart(void)
{
    // uart_tx-P2.2 uart_rx-P2.1
	gpioConfig(GPIO_P2, GPIO_PIN_2, Output_PP);
	gpioConfig(GPIO_P2, GPIO_PIN_1, Input_PullUp_NonSchmitt);
	GPIO_PIN_MAP(TXD_MAP, GPIO_P2, 2);
	GPIO_PIN_MAP(RXD_MAP, GPIO_P2, 1);
    
	uart1Config();
}

/*
* ���룺
* �����
* ���ܣ�lcd������ʼ��
*/
void initLcdDriver(void)
{
    // LCD_CS-P0.5, LCD_W-P0.4, LCD_IO-0.3
    gpioConfig(GPIO_P0, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, Output_PP);
    
    fm1906Config();
    //ht1621_init();
}

/*
* ���룺
* �����
* ���ܣ�lcd������ʼ��
*/
void initTemperatureMeasure(void)
{
    readTempeartureParam(&param);
}

/*
* ���룺
* �����
* ���ܣ�ȥ�����ֵ����Сֵ�˲�
*/
static uint16_t filterMaxMin(void)
{
	uint16_t max = 0;
	uint16_t min = 0xfff;
	uint16_t sum = 0;
	int i = 0;
	
	for(i = 0; i < MAX_PTR; i++)
	{
		sum += adcResultBuffer[i];
		if(max < adcResultBuffer[i])
		{
			max = adcResultBuffer[i];
		}
		
		if(min > adcResultBuffer[i])
		{
			min = adcResultBuffer[i];
		}
	}
	
	sum -= (min + max);
	
	return sum;
}


/*
* ���룺
* �����
* ���ܣ�����ĵ�ص�ѹ
*/
static void calcBatteryVoltage(void)
{
	uint16_t sum = filterMaxMin();
	
	// batteryVoltage = (sum / 4095) * 2 * 2;
	sum >>= 10;
	
	if(sum >= 42)
	{
		batteryVoltageLv = IconBatLv3;
	}
	else if(sum >= 39)
	{
		batteryVoltageLv = IconBatLv2;
	}
	else if(sum >= 34)
	{
		batteryVoltageLv = IconBatLv1;
	}
	else
	{
		batteryVoltageLv = IconBatLv0;
	}
}

/*
* ���룺
* �����
* ���ܣ����ص�ص�ѹ�ȼ�
*/
uint8_t getBatteryVoltageLv(void)
{
	return batteryVoltageLv;
}

/*
* ���룺
* �����
* ���ܣ������¶�ֵ
*/
int16_t getTemperature(void)
{
	return temperature;
}

/*
* ���룺
* �����
* ���ܣ�������¶�ֵ
*/
static void calcTemperature(void)
{
	int32_t sum = filterMaxMin();
    
    // 12λAD���ο���ѹ2V��ʵ�ʵ�ѹֵ*10 = sum / 4095 * 2
    sum *= 100;
    sum >>= 11;
    
    sum = sum * param.k;
    sum = sum - (int32_t)100 * param.b;
    
    temperature = sum / 1000;
}

/*
* ���룺
* �����
* ���ܣ��л�ADCת��ͨ��ͨ��
*/
void switchAdcChannel(void)
{	
	if(ptr >= MAX_PTR)
	{
		if(ch == BatteryVoltage)
		{
			calcBatteryVoltage();
			ch = Temperature;
			XCH_SEL((uint8_t)Temperature);			
		}
		else
		{
			calcTemperature();
			ch = BatteryVoltage;
			XCH_SEL((uint8_t)BatteryVoltage);			
		}
			
		ptr = 0;
		delay_us(10);
	}
	
	START_CONVERT();
}

/*
* ���룺
* �����
* ���ܣ�ADCģ���ʼ��
*/
void adcIsr() interrupt ADC_VECTOR
{	
	ADCC0 &= ~0x20;		//���ADC�жϱ�־λ
	adcResultBuffer[ptr++] = ADCR;
}


/*
 ������
 ���أ�
 ���ܣ�����������
*/	
void powerOn(void)
{
    // P1.0����Ϊ�������
	gpioConfig(GPIO_P1, GPIO_PIN_0, Output_PP);
    P1_0 = 1;
}

/*
 ������
 ���أ�
 ���ܣ��ر���������
*/	
void powerOff(void)
{
	P1_0 = 0;
	
	while(1)
	{}
}


/*
 ������r-��ɫ������g-��ɫ������b-��ɫ������brightness-���Ȱٷֱ�(0-100)
 ���أ�
 ���ܣ�����RGB�Ƶ���ɫ
*/	
void setRGB(uint8_t r, uint8_t g, uint8_t b, uint16_t brightness)
{
    // �����µ���ɫ���ж��в��ٽ��к���Ч���仯
    led.colorChanged = 0;
   
    
    if(brightness > 1000)
    {
        brightness = 1000;
    }
    
    led.red = (uint16_t)r * brightness / 255;
    led.green = (uint16_t)g * brightness / 255;
    led.blue = (uint16_t)b * brightness / 255;
    led.redStep = led.red / STEPS;
    led.greenStep = led.green / STEPS;
    led.blueStep = led.blue / STEPS;
    
    setPwm2Duty(led.red);     
    setPwm0Duty(led.green);     
    setPwm1Duty(led.blue);  
    
    led.currentRed = led.red;
    led.currentGreen = led.green;
    led.currentBlue = led.blue;
    
    // �ж��п�ʼ����Ч���仯
    led.colorChanged = 2;
}




// end of file

