#include "LED.h"

//函数功能：对灯条缓存区进去重置清零
//函数参数：无
//函数返回：无
void clean_led(){
  uint8_t i;
  for(i=0;i<180;i++){color_cache[i]=0;}
}

//函数功能：对灯条缓存区进行编辑，设置某个灯的颜色
//函数参数：灯的编号(0-59)，对应的RGB值(0-255)
//函数返回：无
void set_led(uint8_t num,uint8_t red,uint8_t green,uint8_t blue){
  color_cache[num*3]=green;
  color_cache[num*3+1]=red;
  color_cache[num*3+2]=blue;
}

//函数功能：灯条显示一帧缓存区数据
//函数参数：无
//函数返回：无
#pragma optimize=none
void display_led(){
  uint16_t i,j;
  uint8_t list[8]={0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};
  for(i=0;i<180;i++){//625ns
    for(j=0;j<8;j++){
      if(color_cache[i]&list[j]){//1
        LED_HIGH;//一个nop62ns左右
        asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");//666-708ns,标准要800左右
        asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
        LED_LOW;//625，标准要400左右,只能要三个指令，自加，对比，判断
      }
      else{//0
        LED_HIGH;//0.375ns，标准要300左右
        asm("nop");asm("nop");asm("nop");asm("nop");
        LED_LOW;//625ns，标准要800，浮动700ns到1000ns
        asm("nop");asm("nop");asm("nop");
      }
    }
  }
}

//函数功能：在灯条上显示一帧的时间
//函数参数：24小时制的时分秒
//函数返回：无
void show_time(uint8_t hour,uint8_t minute,uint8_t second){
  uint8_t i,accuracy_hour;
  if(hour>11){
    accuracy_hour=(hour-12)*5+(minute/12);//12:00-23:59
  }
  else{
    accuracy_hour=hour*5+(minute/12);//00:00-11:59
  }
  
  clean_led();
  
  for(i=0;i<60;i++){set_led(i,5,5,5);}
  for(i=1;i<13;i++){set_led(i*5-1,5,20,5);}//12个定位点
  
  if(second<=30){set_led(second+29,0,0,20);}//秒,先画秒，毕竟分针确实是在秒针之上
    else{set_led(second-31,0,0,20);}
  if(minute<=30){set_led(minute+29,0,75,60);}//分
    else{set_led(minute-31,0,75,60);}
  if(accuracy_hour<=30){set_led(accuracy_hour+29,75,75,0);}//时
    else{set_led(accuracy_hour-31,75,75,0);}
  
  display_led();
}

//函数功能：在灯条上显示一个旋转的光圈，需要自定义拖尾
//函数参数：旋转方向(0顺时针，1逆时针)，自定义拖尾的数组，拖尾的长度(最长25)，旋转的速度(每移动一个像素的时间，单位ms)
//函数返回：无
void show_recolve(uint8_t direction,uint8_t tail[],uint8_t length,uint16_t speed){
  uint8_t i,j,led_position,tail_position;
  for(i=0;i<60;i++){
    clean_led();

    for(j=0;j<length;j++){
      if(direction==0){//顺时针
        led_position=(i+j)*3;
        tail_position=(length-j-1)*3;

        if(led_position>=180){led_position=led_position-180;}//如果超出一个循环,超出255就错了，所以拖尾最长25
        color_cache[led_position]=tail[tail_position];
        color_cache[led_position+1]=tail[tail_position+1];
        color_cache[led_position+2]=tail[tail_position+2];
        }
      else{//逆时针
        led_position=(59-i-j)*3+(25*3);//100是转到头的时候用来衔接下一圈的，但是大小有限，所以逆时针的时候拖尾最长25
        tail_position=(length-j-1)*3;

        if(led_position<=75){led_position=led_position+180;}//如果超出了75，也就是我定的"临界线"，就将它重新加180，也就是转移到灯带的末端
        color_cache[led_position-(25*3)]=tail[tail_position];
        color_cache[led_position+1-(25*3)]=tail[tail_position+1];
        color_cache[led_position+2-(25*3)]=tail[tail_position+2];
      }
    }

    display_led();
    delay(speed);
  }
}

//函数功能：在灯条上显示两个旋转的光圈，需要自定义拖尾
//函数参数：旋转方向(0顺时针，1逆时针)，自定义拖尾的数组，拖尾的长度，旋转的速度(每移动一个像素的时间，单位ms)
//函数返回：无
void show_double_recolve(uint8_t direction,uint8_t tail_1[],uint8_t tail_2[],uint8_t length,uint16_t speed){
  uint8_t i,j;
  uint16_t led_position,tail_position;
  for(i=0;i<60;i++){
    clean_led();

    for(j=0;j<length;j++){
      if(direction==0){//顺时针
        tail_position=(length-j-1)*3;

        led_position=(i+j)*3;
        if(led_position>=180){led_position=led_position-180;}
        color_cache[led_position]=tail_1[tail_position];
        color_cache[led_position+1]=tail_1[tail_position+1];
        color_cache[led_position+2]=tail_1[tail_position+2];

        //将数据定义为16位，那么多出来的就可以作为累加的使用，180+90=270；
        //而且每一圈都会重置，所以不担心溢出，而且拖尾的长度就可以更长;
        //如果有必要，可以把单环旋转的这部分也改成16位
        led_position=(i+j)*3+90;
        if(led_position>=180){led_position=led_position-180;}
        color_cache[led_position]=tail_2[tail_position];
        color_cache[led_position+1]=tail_2[tail_position+1];
        color_cache[led_position+2]=tail_2[tail_position+2];
        }
      else{//逆时针的先不写
        // tail_position=(length-j-1)*3;

        // led_position=(59-i-j)*3+180;
        // if(led_position<=180){led_position=led_position+180;}
        // color_cache[led_position-180]=tail_1[tail_position];
        // color_cache[led_position+1-180]=tail_1[tail_position+1];
        // color_cache[led_position+2-180]=tail_1[tail_position+2];

        // led_position=(59-i-j)*3+180+90;
        // if(led_position<=180){led_position=led_position+180;}
        // color_cache[led_position-180]=tail_1[tail_position];
        // color_cache[led_position+1-180]=tail_1[tail_position+1];
        // color_cache[led_position+2-180]=tail_1[tail_position+2];
        return;
      }
    }

    display_led();
    delay(speed);
  }
}

//函数功能：在灯条上显示三个旋转的光圈，需要自定义拖尾
//函数参数：旋转方向(0顺时针，1逆时针)，自定义拖尾的数组，拖尾的长度，旋转的速度(每移动一个像素的时间，单位ms)
//函数返回：无
void show_triple_recolve(uint8_t direction,uint8_t tail_1[],uint8_t tail_2[],uint8_t tail_3[],uint8_t length,uint16_t speed){
  uint8_t i,j;
  uint16_t led_position,tail_position;
  for(i=0;i<60;i++){
    clean_led();

    for(j=0;j<length;j++){
      if(direction==0){//顺时针
        tail_position=(length-j-1)*3;

        led_position=(i+j)*3;
        if(led_position>=180){led_position=led_position-180;}
        color_cache[led_position]=tail_1[tail_position];
        color_cache[led_position+1]=tail_1[tail_position+1];
        color_cache[led_position+2]=tail_1[tail_position+2];

        //将数据定义为16位，那么多出来的就可以作为累加的使用，180+90=270；
        //而且每一圈都会重置，所以不担心溢出，而且拖尾的长度就可以更长;
        //如果有必要，可以把单环旋转的这部分也改成16位
        led_position=(i+j)*3+60;
        if(led_position>=180){led_position=led_position-180;}
          color_cache[led_position]=tail_2[tail_position];
          color_cache[led_position+1]=tail_2[tail_position+1];
          color_cache[led_position+2]=tail_2[tail_position+2];
      
        led_position=(i+j)*3+120;
        if(led_position>=180){led_position=led_position-180;}
          color_cache[led_position]=tail_3[tail_position];
          color_cache[led_position+1]=tail_3[tail_position+1];
          color_cache[led_position+2]=tail_3[tail_position+2];
        }
      else{
        return;
      }
    }

    display_led();
    delay(speed);
  }
}

void show_one_color(uint8_t red,uint8_t green,uint8_t blue){
  uint8_t i;
  for(i=0;i<60;i++){
      set_led(i,red,green,blue);
  }
  display_led();
}

void show_color_breath(uint8_t red,uint8_t green,uint8_t blue,uint16_t speed){
  uint8_t i,j;
  for(j=0;j<100;j++){
    for(i=0;i<60;i++){
        set_led(i,(red/100)*j,(green/100)*j,(blue/100)*j);
    }
    display_led();
    delay(speed);
  }
  for(j=100;j>0;j--){
    for(i=0;i<60;i++){
        set_led(i,(red/100)*j,(green/100)*j,(blue/100)*j);
    }
    display_led();
    delay(speed);
  }
}

void show_double_recolve_demo(void){
    uint8_t i;
    uint8_t data_cache_1[36]={0},data_cache_2[36]={0};//拖尾的缓存数组
    
    for(i=0;i<12;i++){
      data_cache_1[i*3]=(150/12)*(12-i)-8;
      data_cache_1[i*3+1]=(0/12)*(12-i);
      data_cache_1[i*3+2]=(150/12)*(12-i)-8;

      data_cache_2[i*3]=(0/12)*(12-i);
      data_cache_2[i*3+1]=(150/12)*(12-i)-8;
      data_cache_2[i*3+2]=(150/12)*(12-i)-8;
    }
    while(1){
      show_double_recolve(0,data_cache_1,data_cache_2,12,20);
    }
}

void show_triple_recolve_demo(void){
    uint8_t i;
    uint8_t data_cache_1[15]={0},data_cache_2[15]={0},data_cache_3[15]={0};//拖尾的缓存数组
    
    for(i=0;i<5;i++){
      data_cache_1[i*3]=(150/5)*(5-i);
      data_cache_1[i*3+1]=(0/5)*(5-i);
      data_cache_1[i*3+2]=(150/5)*(5-i);

      data_cache_2[i*3]=(0/5)*(5-i);
      data_cache_2[i*3+1]=(150/5)*(5-i);
      data_cache_2[i*3+2]=(150/5)*(5-i);
      
      data_cache_3[i*3]=(150/5)*(5-i);
      data_cache_3[i*3+1]=(150/5)*(5-i);
      data_cache_3[i*3+2]=(0/5)*(5-i);
    }
    
    while(1){
      show_triple_recolve(0,data_cache_1,data_cache_2,data_cache_3,5,20);
    }
}