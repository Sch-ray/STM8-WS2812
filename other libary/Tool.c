#include "Tool.h"

//函数功能：初始化所有外设引脚
//函数参数：无
//函数返回：无
void gpio_init(){
  CLK_HSICmd(ENABLE);//开始内部高频RC
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//1分频16MHz
  
  //灯带
  GPIO_Init(LED_PORT, (GPIO_Pin_TypeDef)LED_PIN, GPIO_MODE_OUT_PP_LOW_FAST);//高速拉低
  GPIO_WriteLow(LED_PORT,LED_PIN);

  //时钟模块
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_CLK, GPIO_MODE_OUT_PP_LOW_FAST);//高速输出拉低
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_RST, GPIO_MODE_OUT_PP_LOW_FAST);//高速输出拉低
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_DAT, GPIO_MODE_OUT_PP_LOW_FAST);//高速输出拉低
  
  //串口
  GPIO_Init(UART_PORT, (GPIO_Pin_TypeDef)UART_TX, GPIO_MODE_OUT_PP_HIGH_SLOW);//高速拉低
  GPIO_Init(UART_PORT, (GPIO_Pin_TypeDef)UART_RX, GPIO_MODE_IN_FL_IT);//浮空输入有中断
  
  //LED指示灯
  GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);//高速拉高
  
  //音频输入
  GPIO_Init(ADC_PORT, (GPIO_Pin_TypeDef)ADC_PIN, GPIO_MODE_IN_PU_NO_IT);//浮空输入无中断，AIN4
  
}

//函数功能：初始化串口1
//函数参数：无
//函数返回：无
void uart_init(){
  UART1_Init((u32)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);//接受数据寄存器非空或溢出中断
  UART1_Cmd(ENABLE);
}

//函数功能：初始化ADC
//函数参数：无
//函数返回：无
void adc_init(void){
  ADC1_DeInit();//初始化ADC1所有寄存器
  ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_4,ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE,ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL4, DISABLE);//参数
  ADC1_Cmd(ENABLE);//使能ADC1
  ADC1_StartConversion();//ADC1转换开始
}

//函数功能：发送一串数据给串口1
//函数参数：数组的指针，数组的长度
//函数返回：无
void uart_sendstr(u8 *str,u8 len){
  int i;
  for(i=0;i<len;i++){
    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
    UART1_SendData8(str[i]);
    delay(1);
   }
   return;
}

//函数功能：进行一个粗略的延时，大概1.05ms
//函数参数：需要延时的大小(0-65535)
//函数返回：无
#pragma optimize=none
void delay(uint16_t nCount){
  uint16_t i;
  uint32_t j;
  for(i=0;i<nCount;i++){
    for(j=0;j<545;j++){
      j=j;
    }
  }
}