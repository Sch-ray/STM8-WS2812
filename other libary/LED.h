#ifndef __LED_H
#define __LED_H
#include "stm8s.h"
#include "tool.h"

#define LED_PORT  (GPIOA)
#define LED_PIN   (GPIO_PIN_3)

//为了节约执行指令的时间，直接赋值给寄存器，但是这一组GPIO就不能另作他用
#define LED_HIGH GPIOA->ODR = 0x08
#define LED_LOW  GPIOA->ODR = 0x00

extern uint8_t color_cache[180];

void clean_led(void);
void set_led(uint8_t num,uint8_t red,uint8_t green,uint8_t blue);
void display_led(void);
void show_time(uint8_t hour,uint8_t minute,uint8_t second);
void show_recolve(uint8_t direction,uint8_t tail[],uint8_t length,uint16_t speed);
void show_double_recolve(uint8_t direction,uint8_t tail_1[],uint8_t tail_2[],uint8_t length,uint16_t speed);
void show_triple_recolve(uint8_t direction,uint8_t tail_1[],uint8_t tail_2[],uint8_t tail_3[],uint8_t length,uint16_t speed);
void show_one_color(uint8_t red,uint8_t green,uint8_t blue);
void show_color_breath(uint8_t red,uint8_t green,uint8_t blue,uint16_t speed);

void show_double_recolve_demo(void);
void show_triple_recolve_demo(void);


#endif