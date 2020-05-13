#ifndef __DS1302_H
#define __DS1302_H
#include "stm8s.h"

#define DS1302_PORT  (GPIOC)
#define DS1302_CLK   (GPIO_PIN_3)
#define DS1302_DAT   (GPIO_PIN_4)
#define DS1302_RST   (GPIO_PIN_5)

//DS1302地址定义
#define ds1302_sec_add			0x80		//秒数据地址
#define ds1302_min_add			0x82		//分数据地址
#define ds1302_hr_add			0x84		//时数据地址
#define ds1302_date_add			0x86		//日数据地址
#define ds1302_month_add		0x88		//月数据地址
#define ds1302_day_add			0x8a		//星期数据地址
#define ds1302_year_add			0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

void set_time(uint8_t time[]);
void get_time(uint8_t *time);
void get_hex_time(uint8_t *time);

#endif