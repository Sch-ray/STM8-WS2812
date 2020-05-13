#include "DS1302.h"

//函数功能：向DS1203写一字节的数据
//函数参数：地址，数据
//函数返回：无
void DS1302_Write(uint8_t addr,uint8_t data)  {
  uint8_t w_data,i;
  w_data=addr&0xfe;
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_DAT, GPIO_MODE_OUT_PP_LOW_FAST);//设置为输出
  
  GPIO_WriteHigh(DS1302_PORT,DS1302_RST);//拉高RST开始传输
  for(i=0;i<8;i++){
    if(w_data & 0x01)//设置DAT的值
      GPIO_WriteHigh(DS1302_PORT,DS1302_DAT);
    else
      GPIO_WriteLow(DS1302_PORT,DS1302_DAT);
    GPIO_WriteHigh(DS1302_PORT,DS1302_CLK);//产生CLK信号
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
  GPIO_WriteLow(DS1302_PORT,DS1302_RST);//拉低RST结束传输
}

//函数功能：从DS1203获取一字节的数据
//函数参数：地址
//函数返回：数据值
uint8_t DS1302_Read(uint8_t addr){
  uint8_t w_data,i;
  w_data=addr|0x01;
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_DAT, GPIO_MODE_OUT_PP_LOW_FAST);//设置为输出
  
  GPIO_WriteHigh(DS1302_PORT,DS1302_RST);//拉高RST开始传输
  for(i=0;i<8;i++){
    if(w_data & 0x01)//设置DAT的值
      GPIO_WriteHigh(DS1302_PORT,DS1302_DAT);
    else
      GPIO_WriteLow(DS1302_PORT,DS1302_DAT);
    GPIO_WriteHigh(DS1302_PORT,DS1302_CLK);//产生CLK信号
    GPIO_WriteLow(DS1302_PORT,DS1302_CLK);
    w_data>>=1;
  }
  GPIO_Init(DS1302_PORT, (GPIO_Pin_TypeDef)DS1302_DAT, GPIO_MODE_IN_FL_NO_IT);//设置为输入
  for(i=0;i<8;i++){
    w_data>>=1;
    if(GPIO_ReadInputPin(DS1302_PORT, DS1302_DAT))//获取DAT的值
      w_data|=0x80;
    else
      w_data&=0x7f;
    GPIO_WriteHigh(DS1302_PORT,DS1302_CLK);//产生CLK信号
    GPIO_WriteLow(DS1302_PORT,DS1302_CLK);
  }
  GPIO_WriteLow(DS1302_PORT,DS1302_RST);//拉低RST结束传输
  return w_data;
}

//函数功能：设置DS1302的时间以及其他相关寄存器
//函数参数：时间数组（年月日时分秒星期）
//函数返回：无
void set_time(uint8_t time[]){
  DS1302_Write(ds1302_control_add,0x00);		//关闭写保护 
  DS1302_Write(ds1302_sec_add,0x80);			//暂停时钟
  DS1302_Write(ds1302_year_add,time[0]);		//年 
  DS1302_Write(ds1302_month_add,time[1]);	        //月 
  DS1302_Write(ds1302_date_add,time[2]);		//日 
  DS1302_Write(ds1302_hr_add,time[3]);		        //时 
  DS1302_Write(ds1302_min_add,time[4]);		        //分
  DS1302_Write(ds1302_sec_add,time[5]);		        //秒
  DS1302_Write(ds1302_day_add,time[6]);		        //周 
  DS1302_Write(ds1302_control_add,0x80);		//打开写保护    
}
//函数功能：从DS1203获取时间，注意！！！这玩意返回的居然是16进制的按十分位百分位显示的时间
//函数参数：无
//函数返回：时间值
void get_time(uint8_t *time){
  time[0]=DS1302_Read(ds1302_year_add);		//年 
  time[1]=DS1302_Read(ds1302_month_add);	//月 
  time[2]=DS1302_Read(ds1302_date_add);		//日 
  time[3]=DS1302_Read(ds1302_hr_add);		//时 
  time[4]=DS1302_Read(ds1302_min_add);		//分 
  time[5]=(DS1302_Read(ds1302_sec_add))&0x7f;   //秒，屏蔽秒的第7位，避免超出59
  time[6]=DS1302_Read(ds1302_day_add);		//周 
}

void get_hex_time(uint8_t *time){
  uint8_t time_cache[3];	 
  time_cache[0]=DS1302_Read(ds1302_hr_add);		 
  time_cache[1]=DS1302_Read(ds1302_min_add);		 
  time_cache[2]=(DS1302_Read(ds1302_sec_add))&0x7f; 	 
  //时
  if(time_cache[0]>=0x20){
    time[0]=time_cache[0]-0x20+20;
  }
  else if(time_cache[0]>=0x10){
    time[0]=time_cache[0]-0x10+10;
  }
  else{
    time[0]=time_cache[0];
  }
  //分
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
  //秒
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