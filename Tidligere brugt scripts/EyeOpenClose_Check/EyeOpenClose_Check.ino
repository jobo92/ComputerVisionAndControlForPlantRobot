
#include <Servo.h>

//Servo indersteServo;
Servo ydersteServo;
int posYdre = 100;
//int posinder = 179;

void setup() {
  
  Serial.begin(9600);
  // indersteServo.write(posinder);
  //indersteServo.attach(9);
  ydersteServo.write(posYdre);
  ydersteServo.attach(10);
}

void loop() {
  //lukker
for (posYdre =99; posYdre <= 160; posYdre += 1)
  {
    ydersteServo.write(posYdre);// tell servo to go to position in variable 'pos'
    Serial.println(posYdre);
    delay(150);
  }

  //åbner
  Serial.println("Out of first loop ");
  for (posYdre = 160; posYdre >= 99; posYdre -= 1)
  {
    ydersteServo.write(posYdre);              // tell servo to go to position in variable 'pos'
    Serial.println(posYdre);
    delay(150);
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
    }*/
  }
  
}
