
#include <Servo.h>
//#include <TimerOne.h>

//Servo indersteServo;
Servo ydersteServo;

int posYdre = 100;
//int posinder = 179;


void setup(void) {
  //Timer1.initialize(10000); // uses microseconds, 1 millionth of a second 0.000001
 // Timer1.attachInterrupt(testFun);
//  pinMode(led, OUTPUT);
 Serial.begin(9600);
  // indersteServo.write(posinder);
  ydersteServo.write(posYdre);
  //indersteServo.attach(9);
 ydersteServo.attach(9);
}

//volatile unsigned long blinkCount = 0; // use volatile for shared variables

void blinkLED()
{
for (posYdre =99; posYdre <= 160; posYdre += 1)
  {
    ydersteServo.write(posYdre);// tell servo to go to position in variable 'pos'
    //Serial.println(posYdre);
    delay(70);
  }
  for (posYdre = 160; posYdre >= 99; posYdre -= 1)
  {
    ydersteServo.write(posYdre);              // tell servo to go to position in variable 'pos'
   // Serial.println(posYdre);
    delay(70);
  }
}

void loop(void) {
   Serial.println("start");
 blinkLED();
 Serial.println("slut");
 delay(10000);
  Serial.println("delay over"); 
  
  /*
    for (posinder = 179; posinder >= 140; posinder -= 1)
    {

     posYdre += 1;
     ydersteServo.write(posYdre);
     indersteServo.write(posinder);
    //Serial.println(posinder);
     delay(15);
    }

    for (posinder = 140; posinder <= 179; posinder += 1)
    {
     posYdre -= 1;
     ydersteServo.write(posYdre);
     indersteServo.write(posinder);
    // Serial.println(posinder);
     delay(15);
    }
*/
  
}
