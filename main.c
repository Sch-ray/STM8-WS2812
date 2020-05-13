#include "stm8s.h"
#include "LED.h"
#include "Tool.h"
#include "DS1302.h"
#include "time.h"
//#include "Adc.h"

uint8_t color_cache[180]={0,30,60,0};//数据帧缓存数组，60个*3种颜色
uint8_t cmd_cache[16]={0};//数据缓存
uint8_t cmd_flag=0;//数据长度

void main(void){
  uint8_t public_buffer[4]={0};//临时存储时间，颜色等，公用
  uint8_t i;
  uint8_t data_cache_1[36]={0},data_cache_2[36]={0},data_cache_3[36]={0};//拖尾的缓存数组
  
  for(i=0;i<12;i++){
    data_cache_1[i*3]=(150/12)*(12-i)-8;
    data_cache_1[i*3+1]=(0/12)*(12-i);
    data_cache_1[i*3+2]=(150/12)*(12-i)-8;

    data_cache_2[i*3]=(0/12)*(12-i);
    data_cache_2[i*3+1]=(150/12)*(12-i)-8;
    data_cache_2[i*3+2]=(150/12)*(12-i)-8;
    
    data_cache_3[i*3]=(150/12)*(12-i);
    data_cache_3[i*3+1]=(150/12)*(12-i)-8;
    data_cache_3[i*3+2]=(0/12)*(12-i)-8;
  }
  gpio_init();
  uart_init();
  adc_init();
  uart_sendstr("normal",6);
  enableInterrupts();
  while(1){
        i=ADC1_GetConversionValue()/4;//缩减为8位
        //开启定时器，每100毫秒采集一次
        //此处程序每10ms更新一次，计算当前值和采集值之间的10个平均数作为缓冲显示
//        while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//        UART1_SendData8(i);
        show_one_color(0,0,i);
        delay(10);
      }
  while(0){
    /*
    0、清空不显示
    1、单个灯环旋转，自定义颜色
    2、两个灯环
    3、三个灯环
    4、纯色
    5、纯色呼吸灯
    6、彩虹色循环
    7、时间显示
    8、倒计时
*/
//    if((cmd_cache[0]==0)||(cmd_cache[0]=='0')){
//      for(i=0;i<cmd_flag;i++){cmd_cache[i]=0;}
//      cmd_flag=0;
//      while(1){
//        clean_led();
//        display_led();
//        delay(500);
//        if(cmd_flag!=0){
//          break;
//        }
//      }
//    }

    if(cmd_cache[0]=='1'){
      for(i=0;i<cmd_flag;i++){cmd_cache[i]=0;}
      cmd_flag=0;
      while(1){
        show_recolve(0,data_cache_1,12,20);
        if(cmd_flag!=0){//如果出现了新的数据就跳出循环进行一次判断，否则一直在这个循环里
          break;
        }
      }
    }
    
    if(cmd_cache[0]=='2'){
      for(i=0;i<cmd_flag;i++){cmd_cache[i]=0;}
      cmd_flag=0;
      while(1){
        show_double_recolve(0,data_cache_1,data_cache_2,12,20);
        if(cmd_flag!=0){
          break;
        }
      }
    }
    
    if(cmd_cache[0]=='3'){
      for(i=0;i<cmd_flag;i++){cmd_cache[i]=0;}
      cmd_flag=0;
      while(1){
        show_triple_recolve(0,data_cache_1,data_cache_2,data_cache_3,12,20);
        if(cmd_flag!=0){
          break;
        }
      }
    }
    //第一个需要是ascii发送，之后的最好是hex发送，或者使用符号，比如青色就是4!~~#
    if(cmd_cache[0]=='4'){
      for(i=0;i<3;i++){
        if(cmd_cache[(i+1)*2-1]>'9')
          public_buffer[i]=cmd_cache[(i+1)*2-1]-0x51;
        else
          public_buffer[i]=cmd_cache[(i+1)*2-1]-0x30;
          
        public_buffer[i]<<=4;
        
        if(cmd_cache[(i+1)*2-1]>'9')
          public_buffer[i]+=cmd_cache[(i+1)*2]-0x51;
        else
          public_buffer[i]+=cmd_cache[(i+1)*2]-0x30;
      }
      for(i=0;i<cmd_flag;i++){cmd_cache[i]=0;}
      cmd_flag=0;
      while(1){
        show_one_color(public_buffer[0],public_buffer[1],public_buffer[2]);
        delay(500);
        if(cmd_flag!=0){
          break;
        }
      }
    }
    
    if(cmd_cache[0]=='5'){
      for(i=0;i<3;i++){
        if(cmd_cache[(i+1)*2-1]>'9')
          public_buffer[i]=cmd_cache[(i+1)*2-1]-0x51;
        else
          public_buffer[i]=cmd_cache[(i+1)*2-1]-0x30;
          
        public_buffer[i]<<=4;
        
        if(cmd_cache[(i+1)*2-1]>'9')
          public_buffer[i]+=cmd_cache[(i+1)*2]-0x51;
        else
          public_buffer[i]+=cmd_cache[(i+1)*2]-0x30;
      }
      if(cmd_cache[7]<='9')
        public_buffer[3]=(cmd_cache[7]-0x30)*25;//最大255
      
      for(i=0;i<cmd_flag;i++){cmd_cache[i]=0;}
      cmd_flag=0;
      while(1){
        show_color_breath(public_buffer[0],public_buffer[1],public_buffer[2],public_buffer[3]);
        if(cmd_flag!=0){
          break;
        }
      }
    }
        
    if(cmd_cache[0]=='7'){
      for(i=0;i<cmd_flag;i++){cmd_cache[i]=0;}
      cmd_flag=0;
      while(1){
        get_hex_time(public_buffer);
        show_time(public_buffer[0],public_buffer[1],public_buffer[2]);
        delay(500);
        if(cmd_flag!=0){
          break;
        }
      }
    }
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(u8* file, u32 line){while(1){}}
#endif