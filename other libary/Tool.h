#ifndef __TOOL_H
#define __TOOL_H
#include "stm8s.h"

#define LED_PORT     (GPIOA)
#define LED_PIN      (GPIO_PIN_3)

#define UART_PORT    (GPIOD)
#define UART_RX      (GPIO_PIN_6)
#define UART_TX      (GPIO_PIN_5)

#define DS1302_PORT  (GPIOC)
#define DS1302_CLK   (GPIO_PIN_3)
#define DS1302_DAT   (GPIO_PIN_4)
#define DS1302_RST   (GPIO_PIN_5)

#define ADC_PORT     (GPIOD)
#define ADC_PIN      (GPIO_PIN_3)

void gpio_init(void);
void uart_init(void);
void adc_init(void);
void delay(uint16_t nCount);
void uart_sendstr(u8 *str,u8 len);

#endif