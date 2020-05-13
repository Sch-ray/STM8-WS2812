#include "Tool.h"

//�������ܣ���ʼ��������������
//������������
//�������أ���
void gpio_init(){
  CLK_HSICmd(ENABLE);//��ʼ�ڲ���ƵRC
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);//1��Ƶ16MHz
  
  //�ƴ�
  GPIO_Init(LED_PORT, (GPIO_Pin_TypeDef)LED_PIN, GPIO_MODE_OUT_PP_LOW_FAST);//��������
  GPIO_WriteLow(LED_PORT,LED_PIN);

  //ʱ��ģ��
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_CLK, GPIO_MODE_OUT_PP_LOW_FAST);//�����������
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_RST, GPIO_MODE_OUT_PP_LOW_FAST);//�����������
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_DAT, GPIO_MODE_OUT_PP_LOW_FAST);//�����������
  
  //����
  GPIO_Init(UART_PORT, (GPIO_Pin_TypeDef)UART_TX, GPIO_MODE_OUT_PP_HIGH_SLOW);//��������
  GPIO_Init(UART_PORT, (GPIO_Pin_TypeDef)UART_RX, GPIO_MODE_IN_FL_IT);//�����������ж�
  
  //LEDָʾ��
  GPIO_Init(GPIOB, (GPIO_Pin_TypeDef)GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);//��������
  
  //��Ƶ����
  GPIO_Init(ADC_PORT, (GPIO_Pin_TypeDef)ADC_PIN, GPIO_MODE_IN_PU_NO_IT);//�����������жϣ�AIN4
  
}

//�������ܣ���ʼ������1
//������������
//�������أ���
void uart_init(){
  UART1_Init((u32)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);//�������ݼĴ����ǿջ�����ж�
  UART1_Cmd(ENABLE);
}

//�������ܣ���ʼ��ADC
//������������
//�������أ���
void adc_init(void){
  ADC1_DeInit();//��ʼ��ADC1���мĴ���
  ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_4,ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE,ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL4, DISABLE);//����
  ADC1_Cmd(ENABLE);//ʹ��ADC1
  ADC1_StartConversion();//ADC1ת����ʼ
}

//�������ܣ�����һ�����ݸ�����1
//���������������ָ�룬����ĳ���
//�������أ���
void uart_sendstr(u8 *str,u8 len){
  int i;
  for(i=0;i<len;i++){
    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
    UART1_SendData8(str[i]);
    delay(1);
   }
   return;
}

//�������ܣ�����һ�����Ե���ʱ�����1.05ms
//������������Ҫ��ʱ�Ĵ�С(0-65535)
//�������أ���
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