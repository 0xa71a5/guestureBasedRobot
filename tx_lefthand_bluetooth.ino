void setup() {
  // put your setup code here, to run once:
Serial.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
//Serial.print(analogRead(A0));  
//Serial.print("   ");
//Serial.println(analogRead(A1));
//delay(100);

int y=analogRead(A0);
int x=analogRead(A1);



if(y<700)
{
  if(x>700)
  {
   Serial.print("<I6>"); //Turn left
   
  //delay(50);;  
  }
  else if(x<400)
  {
   Serial.print("<I7>"); //Turn right
   //delay(50);;
  }
  else
  {
   Serial.print("<I1>"); //Stop the robot
   //delay(50);; 
  }
}
else
{
 if(y<800)
{
 Serial.print("<I2>");
 //delay(50);;
}
else if(y<900)
{
  Serial.print("<I3>");
  //delay(50);;
}
else if(y<1000)
{
 Serial.print("<I4>");
 //delay(50);;
}else
{
  Serial.print("<I5>");
  //delay(50);;
}

}

}
