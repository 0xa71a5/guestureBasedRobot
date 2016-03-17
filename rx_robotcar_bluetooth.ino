#include <Servo.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
Servo servo1;//舵机1，第一关节
Servo servo2;//舵机2，第一关节
Servo servo3;//舵机3， 第一关节
Servo servo4;//舵机4，第二关节
Servo servo5;//舵机5，第二关节
Servo servo6;//舵机6，头部
Servo servo7;//舵机7，头部
Servo servo8;//hand
int maxlimit[9]={0,160,115,160,140,170,175,175,150};
int minlimit[9]={0,20,65,20,10,10,5,5,70};
int middleValue[9]={0,77,40,110,80,90,90,160,70};
int angle_data[9];
char data[40];
bool time1=true;
bool state1=false;
String dataStr;
void limit(int index,int &number)
{
if(number>maxlimit[index])
number=maxlimit[index];
if(number<minlimit[index])
number=minlimit[index];
}
void handlerData(String str)
{
  int i;int numdata=0;
  char flag;
 
  if(!(str[0]>='A'&&str[0]<='Z'))
  return;
  
  if(str.length()==0||str.length()>4)
  return;
 
  for(i=1;i<str.length();i++)
  if(str[i]<'0'||str[i]>'9')
  return;

flag=str[0];
  for(i=1;i<str.length();i++)
  {
    int a=int(str[i]-'0');
    numdata=numdata*10+a;
  }

switch(flag)
{
  case 'A':
limit(1,numdata);
servo1.write(numdata);
Serial.print("  A:");Serial.print(numdata);
  break;

  case 'B':
limit(2,numdata);
servo2.write(numdata);
Serial.print("  B:");Serial.print(numdata);
  break;

  case 'C':
limit(3,numdata);
servo3.write(numdata);
Serial.print("  C:");Serial.print(numdata);
  break;

  case 'D':
limit(4,numdata);
servo4.write(numdata);
Serial.print("  D:");Serial.print(numdata);
  break;

  case 'E':
limit(5,numdata);
servo5.write(numdata);
Serial.print("  E:");Serial.print(numdata);
  break;

  case 'F':
limit(8,numdata);
servo8.write(numdata);
Serial.print("  F:");Serial.print(numdata);
  break;

  case 'G':
limit(7,numdata);
servo7.write(numdata);
Serial.print("  G:");Serial.print(numdata);
  break;


  case 'H':
limit(6,numdata);
servo6.write(numdata);
Serial.print("  H:");Serial.println(numdata);
//Serial1.print(" ");
  break;

  case 'I':
if(str[1]=='5')
{
  digitalWrite(33,1);
  digitalWrite(31,1);
}
else if(str[1]=='1')
{
  digitalWrite(31,0);
  digitalWrite(33,0);
}
else if (str[1]=='6')
{
  digitalWrite(31,0);
  digitalWrite(33,1);
}
else if(str[1]=='7')
{
  digitalWrite(31,1);
  digitalWrite(33,0);
}
else
{
  digitalWrite(31,0);
  digitalWrite(33,0);
}






  break;

  default:
  break;
 
}

}

void setup()
{
Serial1.begin(115200);
Serial.begin(115200);
servo1.attach(11);
servo2.attach(2);
servo3.attach(3);
servo4.attach(4);
servo5.attach(5);
servo6.attach(6);
servo7.attach(7);
servo8.attach(8);
servo1.write(40);
servo2.write(90);
servo3.write(100);
servo4.write(50);
servo5.write(90);
servo6.write(90);
servo7.write(110);
servo8.write(30);

pinMode(31,OUTPUT);
pinMode(33,OUTPUT);
 digitalWrite(31,0);
  digitalWrite(33,0);
}
void loop()
{
  if(Serial1.available()!=0)
  {
    char data=Serial1.read();
      if(data=='<')
    {
      state1=true;
    }
    
    
    if(state1)
    {
      while(Serial1.available()==0);
      char data2=Serial1.read();
      dataStr+=data2;
      delayMicroseconds(80);
      while(data2!='>')
      {
        if(Serial1.available()!=0)
        {
          data2=Serial1.read();
          delayMicroseconds(80);
          if(data2=='>')
          break;
          dataStr+=data2;          
        }        
      }
      handlerData(dataStr);
      state1=false;
     Serial1.println(dataStr);
      dataStr="";
      
    }
  
    
    
  }
  
  
  
}
  
*/
