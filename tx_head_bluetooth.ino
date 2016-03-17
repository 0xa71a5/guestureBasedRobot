//This is for the head node


#include <Servo.h>
#include <SPI.h>
String StrToSend="";
byte data[15];
int dangle2=0,dangle1=0;
int mark=0;
int avr_angle=0;
int servo_angle_bias[10]={0,77,40,110,60,90,90,90,90,90} ;  //编号对应的舵机的初始中点角度值,忽略0号，从1开始
float _angle[15];
int  angle2[15];
int angle3[15];
int reangle3[15];
int sensor[5]={10,46,10,38,36};//关节1  关节2  手腕 左手 头部
int  angle_degree_avr[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int angle_convert[15];
int angle_servo[15];
int  angle_degree[15];  //转换前和转换后的以度为单位的角度数据
int angle_analog_avr;//读取手指处的关节的电阻器的初始角度值
char random_mark[9]={'!','@','#','$','%','(','&','*',')'};//在通信的时候作为校验位使用
//angle2是传感器原始数据换算之后的度数值
int avr_angle1;
int old_angle_degree[10]={0};
int servo_do(int,int,int);
void get_angle_degree(int );
void setup()
{
  pinMode(13,OUTPUT);
 
 Serial.begin(115200);
 Serial1.begin(115200);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV16);//attention it!
  delay(2);

 pinMode(10,OUTPUT);//Start up the sensor
 for(int i=0;i<10;i++)
  {
   
      get_angle_degree(2);
      delay(2);
    

    digitalWrite(13,1);
    delay(5); 

    
      angle_degree_avr[6]+=angle_degree[6];
    angle_degree_avr[8]+=angle_degree[8];
    digitalWrite(13,0);
    delay(30);
  }


    angle_degree_avr[6]/=10;
     angle_degree_avr[8]/=10;
  

Serial.println(angle_degree_avr[6]);


}

void loop()
{
  get_angle_degree(2);
  
  
  
  
  if((angle_degree[6]-angle_degree_avr[6])>=179)
    angle_degree[6]=angle_degree[6]-angle_degree_avr[6]-360;
  else if((angle_degree[6]-angle_degree_avr[6])<=-179)
    angle_degree[6]=angle_degree[6]-angle_degree_avr[6]+360;
  else
    angle_degree[6]=angle_degree[6]-angle_degree_avr[6];

  //y roll convert
  angle_degree[7]=angle_degree[7]-angle_degree_avr[7];
   if((angle_degree[8]-angle_degree_avr[8])>=179)
    angle_degree[8]=angle_degree[8]-angle_degree_avr[8]-360;
  else if((angle_degree[8]-angle_degree_avr[8])<=-179)
    angle_degree[8]=angle_degree[8]-angle_degree_avr[8]+360;
  else
    angle_degree[8]=angle_degree[8]-angle_degree_avr[8];

  
  
  
  
  
  
  angle_servo[6]=servo_do(90,angle_degree[6],1);//6号舵机 头部
  angle_servo[7]=servo_do(90,angle_degree[8],1); //，7号舵机 头部，
  
  
  /*
  Serial.print(angle_servo[6]);
  Serial.print("  ");
  Serial.print(angle_servo[7]);
  Serial.print("  ");
 // Serial.print(angle_degree[8]);
  Serial.println("  ");
  */
  StrToSend="";
  while(Serial1.available()!=0)
  {
    StrToSend+=char(Serial1.read());
delayMicroseconds(100);    
  }
  Serial.println(StrToSend);
  Serial1.print("1");

  
  delay(80);
  
  
  
}

void enable_sensor(int num)//it enables sensor numbered
{
 
  digitalWrite(sensor[num],0);
  delay(2);
}


void get_angle_degree(int num=0)
{
  enable_sensor(num); 
  SPI.transfer(0x00);
  for(int i =0;i<12;i++)
  {
    data[i]=SPI.transfer(0xFF);
  }

  _angle[3*num]=*(float*)&data[0];
  _angle[3*num+1]=*(float*)&data[4];
  _angle[3*num+2]=*(float*)&data[8];

  angle_degree[3*num]=int(_angle[3*num]/0.01744);
  angle_degree[3*num+1]=int(_angle[3*num+1]/0.01744);
  angle_degree[3*num+2]=int(_angle[3*num+2]/0.01744); 

}


int servo_do(int servo_angle_bias,int num2,int sign)
{
 if((servo_angle_bias+sign*num2)>177)
 return 177;
 
if((servo_angle_bias+sign*num2)<10)
 return 10;
  
 return servo_angle_bias+sign*num2;
}

