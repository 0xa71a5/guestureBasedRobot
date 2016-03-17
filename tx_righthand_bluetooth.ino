#include <Servo.h>
#include <SPI.h>
byte data[15];
char data32[32];
int dangle2=0,dangle1=0;
int last_angle_servo[6];
int mark=0;
int avr_angle=0;
int servo_angle_bias[10]={0,77,40,110,60,90,90,90,90,90} ;  //编号对应的舵机的初始中点角度值,忽略0号，从1开始
float _angle[15];
int  angle2[15];
int angle3[15];
int reangle3[15];
int sensor[5]={9,10,6,38,36};//关节1  关节2  手腕 左手 头部
int  angle_degree_avr[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int angle_convert[15];
int angle_servo[15];
int  angle_degree[15];  //转换前和转换后的以度为单位的角度数据
int angle_analog_avr;//读取手指处的关节的电阻器的初始角度值
char random_mark[9]={'!','@','#','$','%','(','&','*',')'};//在通信的时候作为校验位使用
//angle2是传感器原始数据换算之后的度数值
int avr_angle1;
int old_angle_degree[10]={0};
String server2_data="";
int servo_do(int,int,int);
void get_angle_degree(int );
void setup()
{
  pinMode(13,OUTPUT);    
digitalWrite(sensor[0],1);
delay(2);
digitalWrite(sensor[1],1);
delay(2);
pinMode(6,OUTPUT);

 Serial.begin(115200);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV16);//attention it!
  delay(2);
pinMode(9,OUTPUT);
 pinMode(10,OUTPUT);//Start up the sensor
 for(int i=0;i<10;i++)
  {
      get_angle_degree(0);
      delay(2);
      get_angle_degree(1);
      delay(2);
    digitalWrite(13,1);
    delay(5); 
    angle_degree_avr[0]+=angle_degree[0];
    angle_degree_avr[1]+=angle_degree[1];
    angle_degree_avr[2]+=angle_degree[2];
    angle_degree_avr[3]+=angle_degree[3];
    angle_degree_avr[4]+=angle_degree[4];
    angle_degree_avr[5]+=angle_degree[5];
    
    digitalWrite(13,0);
    delay(5);
  }
    angle_degree_avr[0]/=10;
    angle_degree_avr[1]/=10;
    angle_degree_avr[2]/=10; 

    
    angle_degree_avr[3]/=10; 
    angle_degree_avr[4]/=10; 
    angle_degree_avr[5]/=10; 
    
    
    
last_angle_servo[1]=40;
last_angle_servo[2]=90;
last_angle_servo[3]=100;
last_angle_servo[4]=50;
last_angle_servo[5]=90;

}

void loop()
{
 
  get_angle_degree(0);
  get_angle_degree(1);//获取右边胳膊上两个传感器的数值
  
  
  if((angle_degree[0]-angle_degree_avr[0])>=179)
    angle_degree[0]=angle_degree[0]-angle_degree_avr[0]-360;
  else if((angle_degree[0]-angle_degree_avr[0])<=-179)
    angle_degree[0]=angle_degree[0]-angle_degree_avr[0]+360;
  else
    angle_degree[0]=angle_degree[0]-angle_degree_avr[0];

  //y roll convert
  angle_degree[1]=angle_degree[1]-angle_degree_avr[1];

  //z roll convert
  if((angle_degree[2]-angle_degree_avr[2])>=179)
    angle_degree[2]=angle_degree[2]-angle_degree_avr[2]-360;
  else if((angle_degree[2]-angle_degree_avr[2])<=-179)
    angle_degree[2]=angle_degree[2]-angle_degree_avr[2]+360;
  else
    angle_degree[2]=angle_degree[2]-angle_degree_avr[2];

 if((angle_degree[3]-angle_degree_avr[3])>=179)
    angle_degree[3]=angle_degree[3]-angle_degree_avr[3]-360;
  else if((angle_degree[3]-angle_degree_avr[3])<=-179)
    angle_degree[3]=angle_degree[3]-angle_degree_avr[3]+360;
  else
    angle_degree[3]=angle_degree[3]-angle_degree_avr[3];

  //y roll convert
  angle_degree[4]=angle_degree[4]-angle_degree_avr[4];

  //z roll convert
  if((angle_degree[5]-angle_degree_avr[5])>=179)
    angle_degree[5]=angle_degree[5]-angle_degree_avr[5]-360;
  else if((angle_degree[5]-angle_degree_avr[5])<=-179)
    angle_degree[5]=angle_degree[5]-angle_degree_avr[5]+360;
  else
    angle_degree[5]=angle_degree[5]-angle_degree_avr[5];



  angle_servo[1]=servo_do(40,angle_degree[2],1);//1号舵机 关节1，
  angle_servo[2]=servo_do(90,angle_degree[0],-1); //2号舵机关节1
  angle_servo[3]=servo_do(100,angle_degree[1],1); //,3号舵机 关节1
  angle_servo[4]=servo_do(50,angle_degree[5],-1);//4号舵机
  angle_servo[5]=servo_do(90,angle_degree[4],1);//5号舵机

int ccAnalog=analogRead(A0);
if(ccAnalog>650)
ccAnalog=650;
if(ccAnalog<510)
ccAnalog=510;
angle_servo[6]=map(ccAnalog,510,650,70,150);//获取手指上的电位器的读数并map下


int diff[6];
diff[0]=0;
diff[1]=angle_servo[1]-last_angle_servo[1];
diff[2]=angle_servo[2]-last_angle_servo[2];
diff[3]=angle_servo[3]-last_angle_servo[3];
diff[4]=angle_servo[4]-last_angle_servo[4];
diff[5]=angle_servo[5]-last_angle_servo[5];

                                        //计算前后两次传感器数据的差值
for(int i=1;i<6;i++)
{
  if (diff[i]>19||diff[i]<-19)
  {
    angle_servo[i]=last_angle_servo[i];
   }
}
                                      //如果检测到突变就放弃此次数据
int changeServo1=angle_servo[1]-40;   //牵连角度的计算
//angle_servo[4]+=changeServo1;



last_angle_servo[1]=angle_servo[1];
last_angle_servo[2]=angle_servo[2];
last_angle_servo[3]=angle_servo[3];
last_angle_servo[4]=angle_servo[4];
last_angle_servo[5]=angle_servo[5];

server2_data="<A";
server2_data+=angle_servo[1];server2_data+="><B";
server2_data+=angle_servo[2];server2_data+="><C";
server2_data+=angle_servo[3];server2_data+="><D";
server2_data+=(angle_servo[4]+changeServo1-5);server2_data+=">";
//对传感器数据进行编码 

/*
for(int i=0;i<server2_data.length();i++)
data32[i]=server2_data[i];
for(int i=server2_data.length();i<32;i++)//将长度不足的位进行NULL填充
data32[i]=NULL;
  Mirf.send((byte *)&data32);       //发送数据
 while(Mirf.isSending());           //等待数据发送完成
  */

server2_data+="<E";
server2_data+=angle_servo[5];server2_data+="><F";
server2_data+=angle_servo[6];server2_data+=">";
/*
for(int i=0;i<server2_data.length();i++)
data32[i]=server2_data[i];
for(int i=server2_data.length();i<32;i++)
data32[i]=NULL;
Mirf.send((byte *)&data32);
 while(Mirf.isSending());         //因为一次发送的数据的长度不能超过32byte
                                  //所以分两次发送
*/
Serial.print(server2_data);

long int temptime1=millis();
while(Serial.available()==0)
{
  if(millis()-temptime1>500)
  break;
}

while(Serial.available()!=0)
{
  char tempdata=Serial.read();
delayMicroseconds(60);  
}

delay(60);





}

void enable_sensor(int num)//it enables sensor numbered
{
digitalWrite(sensor[0],1);
delay(2);
digitalWrite(sensor[1],1);
delay(2);
digitalWrite(sensor[2],1);
delay(2);

  
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

