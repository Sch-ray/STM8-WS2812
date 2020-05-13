#ifndef __DS1302_H
#define __DS1302_H
#include "stm8s.h"

#define DS1302_PORT  (GPIOC)
#define DS1302_CLK   (GPIO_PIN_3)
#define DS1302_DAT   (GPIO_PIN_4)
#define DS1302_RST   (GPIO_PIN_5)

//DS1302��ַ����
#define ds1302_sec_add			0x80		//�����ݵ�ַ
#define ds1302_min_add			0x82		//�����ݵ�ַ
#define ds1302_hr_add			0x84		//ʱ���ݵ�ַ
#define ds1302_date_add			0x86		//�����ݵ�ַ
#define ds1302_month_add		0x88		//�����ݵ�ַ
#define ds1302_day_add			0x8a		//�������ݵ�ַ
#define ds1302_year_add			0x8c		//�����ݵ�ַ
#define ds1302_control_add		0x8e		//�������ݵ�ַ
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

void set_time(uint8_t time[]);
void get_time(uint8_t *time);
void get_hex_time(uint8_t *time);

#endif