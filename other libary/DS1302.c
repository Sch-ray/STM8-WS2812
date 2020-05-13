#include "DS1302.h"

//�������ܣ���DS1203дһ�ֽڵ�����
//������������ַ������
//�������أ���
void DS1302_Write(uint8_t addr,uint8_t data)  {
  uint8_t w_data,i;
  w_data=addr&0xfe;
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_DAT, GPIO_MODE_OUT_PP_LOW_FAST);//����Ϊ���
  
  GPIO_WriteHigh(DS1302_PORT,DS1302_RST);//����RST��ʼ����
  for(i=0;i<8;i++){
    if(w_data & 0x01)//����DAT��ֵ
      GPIO_WriteHigh(DS1302_PORT,DS1302_DAT);
    else
      GPIO_WriteLow(DS1302_PORT,DS1302_DAT);
    GPIO_WriteHigh(DS1302_PORT,DS1302_CLK);//����CLK�ź�
    GPIO_WriteLow(DS1302_PORT,DS1302_CLK);
    w_data>>=1;
  }
  w_data=data;
  for(i=0;i<8;i++){
    if(w_data & 0x01)
      GPIO_WriteHigh(DS1302_PORT,DS1302_DAT);
    else
      GPIO_WriteLow(DS1302_PORT,DS1302_DAT);
    GPIO_WriteHigh(DS1302_PORT,DS1302_CLK);
    GPIO_WriteLow(DS1302_PORT,DS1302_CLK);
    w_data>>=1;
  }
  GPIO_WriteLow(DS1302_PORT,DS1302_RST);//����RST��������
}

//�������ܣ���DS1203��ȡһ�ֽڵ�����
//������������ַ
//�������أ�����ֵ
uint8_t DS1302_Read(uint8_t addr){
  uint8_t w_data,i;
  w_data=addr|0x01;
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_DAT, GPIO_MODE_OUT_PP_LOW_FAST);//����Ϊ���
  
  GPIO_WriteHigh(DS1302_PORT,DS1302_RST);//����RST��ʼ����
  for(i=0;i<8;i++){
    if(w_data & 0x01)//����DAT��ֵ
      GPIO_WriteHigh(DS1302_PORT,DS1302_DAT);
    else
      GPIO_WriteLow(DS1302_PORT,DS1302_DAT);
    GPIO_WriteHigh(DS1302_PORT,DS1302_CLK);//����CLK�ź�
    GPIO_WriteLow(DS1302_PORT,DS1302_CLK);
    w_data>>=1;
  }
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_DAT, GPIO_MODE_IN_FL_NO_IT);//����Ϊ����
  for(i=0;i<8;i++){
    w_data>>=1;
    if(GPIO_ReadInputPin(DS1302_PORT, DS1302_DAT))//��ȡDAT��ֵ
      w_data|=0x80;
    else
      w_data&=0x7f;
    GPIO_WriteHigh(DS1302_PORT,DS1302_CLK);//����CLK�ź�
    GPIO_WriteLow(DS1302_PORT,DS1302_CLK);
  }
  GPIO_WriteLow(DS1302_PORT,DS1302_RST);//����RST��������
  return w_data;
}

//�������ܣ�����DS1302��ʱ���Լ�������ؼĴ���
//����������ʱ�����飨������ʱ�������ڣ�
//�������أ���
void set_time(uint8_t time[]){
  DS1302_Write(ds1302_control_add,0x00);		//�ر�д���� 
  DS1302_Write(ds1302_sec_add,0x80);			//��ͣʱ��
  DS1302_Write(ds1302_year_add,time[0]);		//�� 
  DS1302_Write(ds1302_month_add,time[1]);	        //�� 
  DS1302_Write(ds1302_date_add,time[2]);		//�� 
  DS1302_Write(ds1302_hr_add,time[3]);		        //ʱ 
  DS1302_Write(ds1302_min_add,time[4]);		        //��
  DS1302_Write(ds1302_sec_add,time[5]);		        //��
  DS1302_Write(ds1302_day_add,time[6]);		        //�� 
  DS1302_Write(ds1302_control_add,0x80);		//��д����    
}
//�������ܣ���DS1203��ȡʱ�䣬ע�⣡���������ⷵ�صľ�Ȼ��16���Ƶİ�ʮ��λ�ٷ�λ��ʾ��ʱ��
//������������
//�������أ�ʱ��ֵ
void get_time(uint8_t *time){
  time[0]=DS1302_Read(ds1302_year_add);		//�� 
  time[1]=DS1302_Read(ds1302_month_add);	//�� 
  time[2]=DS1302_Read(ds1302_date_add);		//�� 
  time[3]=DS1302_Read(ds1302_hr_add);		//ʱ 
  time[4]=DS1302_Read(ds1302_min_add);		//�� 
  time[5]=(DS1302_Read(ds1302_sec_add))&0x7f;   //�룬������ĵ�7λ�����ⳬ��59
  time[6]=DS1302_Read(ds1302_day_add);		//�� 
}

void get_hex_time(uint8_t *time){
  uint8_t time_cache[3];	 
  time_cache[0]=DS1302_Read(ds1302_hr_add);		 
  time_cache[1]=DS1302_Read(ds1302_min_add);		 
  time_cache[2]=(DS1302_Read(ds1302_sec_add))&0x7f; 	 
  //ʱ
  if(time_cache[0]>=0x20){
    time[0]=time_cache[0]-0x20+20;
  }
  else if(time_cache[0]>=0x10){
    time[0]=time_cache[0]-0x10+10;
  }
  else{
    time[0]=time_cache[0];
  }
  //��
  if(time_cache[1]>=0x50){
    time[1]=time_cache[1]-0x50+50;
  }
  else if(time_cache[1]>=0x40){
    time[1]=time_cache[1]-0x40+40;
  }
  else if(time_cache[1]>=0x30){
    time[1]=time_cache[1]-0x30+30;
  }
  else if(time_cache[1]>=0x20){
    time[1]=time_cache[1]-0x20+20;
  }
  else if(time_cache[1]>=0x10){
    time[1]=time_cache[1]-0x10+10;
  }
  else{
    time[1]=time_cache[1];
  }
  //��
  if(time_cache[2]>=0x50){
    time[2]=time_cache[2]-0x50+50;
  }
  else if(time_cache[2]>=0x40){
    time[2]=time_cache[2]-0x40+40;
  }
  else if(time_cache[2]>=0x30){
    time[2]=time_cache[2]-0x30+30;
  }
  else if(time_cache[2]>=0x20){
    time[2]=time_cache[2]-0x20+20;
  }
  else if(time_cache[2]>=0x10){
    time[2]=time_cache[2]-0x10+10;
  }
  else{
    time[2]=time_cache[2];
  }
}