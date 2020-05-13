#include "LED.h"

//�������ܣ��Ե�����������ȥ��������
//������������
//�������أ���
void clean_led(){
  uint8_t i;
  for(i=0;i<180;i++){color_cache[i]=0;}
}

//�������ܣ��Ե������������б༭������ĳ���Ƶ���ɫ
//�����������Ƶı��(0-59)����Ӧ��RGBֵ(0-255)
//�������أ���
void set_led(uint8_t num,uint8_t red,uint8_t green,uint8_t blue){
  color_cache[num*3]=green;
  color_cache[num*3+1]=red;
  color_cache[num*3+2]=blue;
}

//�������ܣ�������ʾһ֡����������
//������������
//�������أ���
#pragma optimize=none
void display_led(){
  uint16_t i,j;
  uint8_t list[8]={0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};
  for(i=0;i<180;i++){//625ns
    for(j=0;j<8;j++){
      if(color_cache[i]&list[j]){//1
        LED_HIGH;//һ��nop62ns����
        asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");//666-708ns,��׼Ҫ800����
        asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
        LED_LOW;//625����׼Ҫ400����,ֻ��Ҫ����ָ��Լӣ��Աȣ��ж�
      }
      else{//0
        LED_HIGH;//0.375ns����׼Ҫ300����
        asm("nop");asm("nop");asm("nop");asm("nop");
        LED_LOW;//625ns����׼Ҫ800������700ns��1000ns
        asm("nop");asm("nop");asm("nop");
      }
    }
  }
}

//�������ܣ��ڵ�������ʾһ֡��ʱ��
//����������24Сʱ�Ƶ�ʱ����
//�������أ���
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
  for(i=1;i<13;i++){set_led(i*5-1,5,20,5);}//12����λ��
  
  if(second<=30){set_led(second+29,0,0,20);}//��,�Ȼ��룬�Ͼ�����ȷʵ��������֮��
    else{set_led(second-31,0,0,20);}
  if(minute<=30){set_led(minute+29,0,75,60);}//��
    else{set_led(minute-31,0,75,60);}
  if(accuracy_hour<=30){set_led(accuracy_hour+29,75,75,0);}//ʱ
    else{set_led(accuracy_hour-31,75,75,0);}
  
  display_led();
}

//�������ܣ��ڵ�������ʾһ����ת�Ĺ�Ȧ����Ҫ�Զ�����β
//������������ת����(0˳ʱ�룬1��ʱ��)���Զ�����β�����飬��β�ĳ���(�25)����ת���ٶ�(ÿ�ƶ�һ�����ص�ʱ�䣬��λms)
//�������أ���
void show_recolve(uint8_t direction,uint8_t tail[],uint8_t length,uint16_t speed){
  uint8_t i,j,led_position,tail_position;
  for(i=0;i<60;i++){
    clean_led();

    for(j=0;j<length;j++){
      if(direction==0){//˳ʱ��
        led_position=(i+j)*3;
        tail_position=(length-j-1)*3;

        if(led_position>=180){led_position=led_position-180;}//�������һ��ѭ��,����255�ʹ��ˣ�������β�25
        color_cache[led_position]=tail[tail_position];
        color_cache[led_position+1]=tail[tail_position+1];
        color_cache[led_position+2]=tail[tail_position+2];
        }
      else{//��ʱ��
        led_position=(59-i-j)*3+(25*3);//100��ת��ͷ��ʱ�������ν���һȦ�ģ����Ǵ�С���ޣ�������ʱ���ʱ����β�25
        tail_position=(length-j-1)*3;

        if(led_position<=75){led_position=led_position+180;}//���������75��Ҳ�����Ҷ���"�ٽ���"���ͽ������¼�180��Ҳ����ת�Ƶ��ƴ���ĩ��
        color_cache[led_position-(25*3)]=tail[tail_position];
        color_cache[led_position+1-(25*3)]=tail[tail_position+1];
        color_cache[led_position+2-(25*3)]=tail[tail_position+2];
      }
    }

    display_led();
    delay(speed);
  }
}

//�������ܣ��ڵ�������ʾ������ת�Ĺ�Ȧ����Ҫ�Զ�����β
//������������ת����(0˳ʱ�룬1��ʱ��)���Զ�����β�����飬��β�ĳ��ȣ���ת���ٶ�(ÿ�ƶ�һ�����ص�ʱ�䣬��λms)
//�������أ���
void show_double_recolve(uint8_t direction,uint8_t tail_1[],uint8_t tail_2[],uint8_t length,uint16_t speed){
  uint8_t i,j;
  uint16_t led_position,tail_position;
  for(i=0;i<60;i++){
    clean_led();

    for(j=0;j<length;j++){
      if(direction==0){//˳ʱ��
        tail_position=(length-j-1)*3;

        led_position=(i+j)*3;
        if(led_position>=180){led_position=led_position-180;}
        color_cache[led_position]=tail_1[tail_position];
        color_cache[led_position+1]=tail_1[tail_position+1];
        color_cache[led_position+2]=tail_1[tail_position+2];

        //�����ݶ���Ϊ16λ����ô������ľͿ�����Ϊ�ۼӵ�ʹ�ã�180+90=270��
        //����ÿһȦ�������ã����Բ����������������β�ĳ��ȾͿ��Ը���;
        //����б�Ҫ�����԰ѵ�����ת���ⲿ��Ҳ�ĳ�16λ
        led_position=(i+j)*3+90;
        if(led_position>=180){led_position=led_position-180;}
        color_cache[led_position]=tail_2[tail_position];
        color_cache[led_position+1]=tail_2[tail_position+1];
        color_cache[led_position+2]=tail_2[tail_position+2];
        }
      else{//��ʱ����Ȳ�д
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

//�������ܣ��ڵ�������ʾ������ת�Ĺ�Ȧ����Ҫ�Զ�����β
//������������ת����(0˳ʱ�룬1��ʱ��)���Զ�����β�����飬��β�ĳ��ȣ���ת���ٶ�(ÿ�ƶ�һ�����ص�ʱ�䣬��λms)
//�������أ���
void show_triple_recolve(uint8_t direction,uint8_t tail_1[],uint8_t tail_2[],uint8_t tail_3[],uint8_t length,uint16_t speed){
  uint8_t i,j;
  uint16_t led_position,tail_position;
  for(i=0;i<60;i++){
    clean_led();

    for(j=0;j<length;j++){
      if(direction==0){//˳ʱ��
        tail_position=(length-j-1)*3;

        led_position=(i+j)*3;
        if(led_position>=180){led_position=led_position-180;}
        color_cache[led_position]=tail_1[tail_position];
        color_cache[led_position+1]=tail_1[tail_position+1];
        color_cache[led_position+2]=tail_1[tail_position+2];

        //�����ݶ���Ϊ16λ����ô������ľͿ�����Ϊ�ۼӵ�ʹ�ã�180+90=270��
        //����ÿһȦ�������ã����Բ����������������β�ĳ��ȾͿ��Ը���;
        //����б�Ҫ�����԰ѵ�����ת���ⲿ��Ҳ�ĳ�16λ
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
    uint8_t data_cache_1[36]={0},data_cache_2[36]={0};//��β�Ļ�������
    
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
    uint8_t data_cache_1[15]={0},data_cache_2[15]={0},data_cache_3[15]={0};//��β�Ļ�������
    
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