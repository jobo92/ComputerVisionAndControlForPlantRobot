/*
  # Face tracking using arduino - Code #
  # Code by Harsh Dethe
  # 09 Sep 2018.
  Modified by Mathias and Sidsel
*/

#include<Servo.h>
#include<Wire.h>
int kordinatRecived = 0;
unsigned long myTime;

Servo servoVer; //Vertical Servo
Servo servoHor; //Horizontal Servo

int x;
int y;

int prevX;
int prevY;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  servoVer.attach(10); //Attach Vertical Servo to Pin 5
  servoHor.attach(11); //Attach Horizontal Servo to Pin 6
  servoVer.write(4);
  servoHor.write(4); 
}



void Pos()
{
 if(prevX != x || prevY != y)
  {
  
    prevX = x;
    prevY = y;
   //tune this range to generate map
    int servoX = map(x, 600,100, 1, 65); //venstre højre
    //tune this range to generate map
    int servoY = map(y, 430, 40, 60, 1); // op ned
/*
    servoX = min(servoX, 1);
    servoX = max(servoX, 65);
    servoY = min(servoY, 60);
    servoY = max(servoY, 1);
    */
    servoHor.write(servoX);
    servoVer.write(servoY);

     kordinatRecived =1;
    Wire.beginTransmission(9); // transmit to device #9
   
  Wire.write(kordinatRecived);
  Wire.endTransmission();
  myTime = millis();
  
  }
  else if(prevX == x || prevY == y)
  {
    if ((myTime +2000) <= millis())
    {
      kordinatRecived =0;
      Wire.beginTransmission(9); // transmit to device #9
      Wire.write(kordinatRecived);
      Wire.endTransmission();
     } 
  }
  
}

void loop()
{
 
  if(Serial.available() > 0)
  {
    if(Serial.read() == 'X')
    {
      x = Serial.parseInt();
      if(Serial.read() == 'Y')
      {
        y = Serial.parseInt();
  
       Pos();
      
      }
    }
    while(Serial.available() > 0)
    {
      Serial.read();
    }
  } 
}
