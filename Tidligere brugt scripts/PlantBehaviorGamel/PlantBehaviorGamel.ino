//***************************************************************
//*** SOFT ROBOTIC ACTUATOR CONTROL WITH ARDUINO MOTOR SHIELD ***
//***              Jonas Jørgensen                            ***
//***************************************************************
// Based on "Simple Motor Shield sketch" (June 2012, Open Source / Public Domain) By arduino.cc user "Krodal".
// (A simple sketch for the motor shield without using the Adafruit library)

// Connector usage
// ---------------
// The order is different than what you would expect.
// If the Arduino (Uno) board is held with the USB
// connector to the left, the positive (A) side is
// at the top (north), and the negative (B) side is
// the bottom (south) for both headers.
//
//   Connector X1:
//     M1 on outside = MOTOR1_A   (+) north
//     M1 on inside  = MOTOR1_B   (-)
//     middle        = GND
//     M2 on inside  = MOTOR2_A   (+)
//     M2 on outside = MOTOR2_B   (-) south
//
//   Connector X2:
//     M3 on outside = MOTOR3_B   (-) south
//     M3 on inside  = MOTOR3_A   (+)
//     middle        = GND
//     M4 on inside  = MOTOR4_B   (-)
//     M4 on outside = MOTOR4_A   (+) north
//
//
//         -------------------------------
//         | -+s                         |
//         | -+s                         |
//    M1 A |                             | M4 A
//    M1 B |                             | M4 B
//    GND  |                             | GND
//    M2 A |                             | M3 A
//    M2 B |                             | M3 B
//         |                       ..... |
//         -------------------------------
//                + -
//
//
//
// Pin usage with the Motorshield
// ---------------------------------------
// Analog pins: not used at all
//     A0 ... A5 are still available
//     They all can also be used as digital pins.
//     Also I2C (A4=SDA and A5=SCL) can be used.
//     These pins have a breadboard area on the shield.
// Digital pins: used: 3,4,5,6,7,8,9,10,11,12
//     Pin 9 and 10 are only used for the servo motors.
//     Already in use: 0 (RX) and 1 (TX).
//     Unused: 2,13
//     Pin 2 has an soldering hole on the board,
//           easy to connect a wire.
//     Pin 13 is also connected to the system led.
// I2C is possible, but SPI is not possible since
// those pins are used.
//


#include <Servo.h>
#include <Wire.h>


// Arduino pins for the shift register
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

// 8-bit bus after the 74HC595 shift register
// (not Arduino pins)
// These are used to set the direction of the bridge driver.
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

// Arduino pins for the PWM signals.
#define MOTOR1_PWM 11
#define MOTOR2_PWM 3
#define MOTOR3_PWM 6
#define MOTOR4_PWM 5
#define SERVO1_PWM 10
#define SERVO2_PWM 9

// Codes for the motor function.
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

//INTERESTING STUFF BELOW THIS LINE!!!!!!!!!!!!!!!

//booleans to keep track of what behaviors to execute
boolean isEyeActive = false;
boolean isPlantDead = false;

boolean isStart = true;

//int to keep track of behavior
int behaviorType = 1;
int shouldChangeBehavior = 0;

int LED = 13;
int x = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Soft robotic actuator control w. Arduino motor shield");
// Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}
void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C
}

void loop()
{
//If value received is 0 blink LED for 200 ms
  if (x == 1) 
  {
   isEyeActive = true; 
  }
  else if (x==0)
  {
     isEyeActive = false;
   }
  
  
  //Normally-closed valves are used
  
  //***PUMP1: connected to M1_A(+) and M1_B(-)
  //***VALVE1: connected to M2_A(+) and M2_B(-)
  
  //***PUMP2: connected to M4_A(+) and M4_B(-)
  //***VALVE2: connected to M3_A(+) and M3_B(-)

  // ----------- actual code here ----------------

  
  
  if(isStart == true)
  {
    motor_output(MOTOR2_A, HIGH, 255); //Open VALVE1 
    motor_output(MOTOR3_A, HIGH, 255); //Open VALVE2
    delay(3000); //Delay to release trapped air 
    isStart = false;
  }
  
  
  //different states of behavior
  if(isEyeActive == false)
  {
    //if the eye is inactive, commence idle behaviors
    motor_output(MOTOR2_A, LOW, 255); //Close VALVE1
    motor_output(MOTOR3_A, LOW, 255); //Close VALVE2
    
    motor(1, FORWARD, 255); //Start PUMP1 
    motor(4, FORWARD, 255); //Start PUMP2 
    //delay(random(1000, 4000));
    delay(5000);
    motor(1, RELEASE, 0); //Stop PUMP1
    motor(4, RELEASE, 0); //Stop PUMP2
    delay(random(0, 10000));

    motor_output(MOTOR2_A, HIGH, 255); //Open VALVE1
    motor_output(MOTOR3_A, HIGH, 255); //Open VALVE2
    //delay(random(1000, 5000));
    delay(10000);
  }
  else if(isEyeActive == true)
  {
    //if eye active, commence active behaviors

    if(behaviorType == 0)
    {
      //Comfortable behavior
      motor_output(MOTOR2_A, LOW, 255); //Close VALVE1
    
      motor(1, FORWARD, 255); //Start PUMP1 
      delay(random(1000, 4000));
      motor(1, RELEASE, 0); //Stop PUMP1
      delay(random(1000, 5000));

      motor_output(MOTOR2_A, HIGH, 255); //Open VALVE1
      delay(random(2000, 5000));
      
    }
    else if(behaviorType == 1)
    {
      //Uncomfortable behavior
      motor_output(MOTOR2_A, LOW, 255); //Close VALVE1
    
      motor(1, FORWARD, 255); //Start PUMP1 
      delay(random(1000, 1500));
      motor(1, RELEASE, 0); //Stop PUMP1

      motor_output(MOTOR2_A, HIGH, 255); //Open VALVE1
      delay(random(400, 800));
      
    }
  }
    /*
     //old active behaviors
    if(behaviorType == 0)
    {
      //make a normal behavior
      motor_output(MOTOR3_A, LOW, 255); //Close VALVE2
    
      motor(1, FORWARD, 255); //Start PUMP1 
      delay(random(1000, 4000));
      motor(1, RELEASE, 0); //Stop PUMP1
      delay(random(0, 15000));

      motor_output(MOTOR3_A, HIGH, 255); //Open VALVE2
      delay(random(1000, 5000));

      shouldChangeBehavior = random(0, 10);
      if(shouldChangeBehavior >= 8){
        behaviorType = random(0, 2);
      }
    }
    else if (behaviorType == 1)
    {
      //make a frantic behavior
      motor_output(MOTOR3_A, LOW, 255); //Close VALVE2
    
      motor(1, FORWARD, 255); //Start PUMP1 
      delay(random(1000, 1500));
      motor(1, RELEASE, 0); //Stop PUMP1

      motor_output(MOTOR3_A, HIGH, 255); //Open VALVE2
      delay(random(400, 800));

      shouldChangeBehavior = random(0, 10);
      if(shouldChangeBehavior >= 5){
        behaviorType = random(0, 2);
      }
    }
    else if (behaviorType == 2)
    {
      //make a heartbeat/breathe behavior?
      motor_output(MOTOR3_A, LOW, 255); //Close VALVE2
    
      motor(1, FORWARD, 255); //Start PUMP1 
      delay(random(2000, 3000));
      motor(1, RELEASE, 0); //Stop PUMP1
      delay(500);

      motor_output(MOTOR3_A, HIGH, 255); //Open VALVE2
      delay(random(1000, 2000));
      delay(500);

      shouldChangeBehavior = random(0, 10);
      if(shouldChangeBehavior >= 6){
        behaviorType = random(0, 2);
      }
    }

  }*/
  else if(isPlantDead == true)
  {
    //if plant dead, do dead behavior
    //stop all motors, open all valves

    motor(1, RELEASE, 0);//Stop PUMP1
    motor(4, RELEASE, 0);//Stop PUMP2

    motor_output(MOTOR2_A, HIGH, 255); //Open VALVE1
    motor_output(MOTOR3_A, HIGH, 255); //Open VALVE2
  }

}

//INTERESTING STUFF ABOVE THIS LINE!!!!!!!!!!!!!!!!!!!!!!!

// Initializing
// ------------
// There is no initialization function.
//
// The shiftWrite() has an automatic initializing.
// The PWM outputs are floating during startup,
// that's okay for the Motor Shield, it stays off.
// Using analogWrite() without pinMode() is valid.
//


// ---------------------------------
// motor
//
// Select the motor (1-4), the command,
// and the speed (0-255).
// The commands are: FORWARD, BACKWARD, BRAKE, RELEASE.

void motor(int nMotor, int command, int speed)
{
  int motorA, motorB;

  if (nMotor >= 1 && nMotor <= 4)
  {  
    switch (nMotor)
    {
    case 1:
      motorA   = MOTOR1_A;
      motorB   = MOTOR1_B;
      break;
    case 2:
      motorA   = MOTOR2_A;
      motorB   = MOTOR2_B;
      break;
    case 3:
      motorA   = MOTOR3_A;
      motorB   = MOTOR3_B;
      break;
    case 4:
      motorA   = MOTOR4_A;
      motorB   = MOTOR4_B;
      break;
    default:
      break;
    }

    switch (command)
    {
    case FORWARD:
      motor_output (motorA, HIGH, speed);
      motor_output (motorB, LOW, -1);     // -1: no PWM set
      break;
    case BACKWARD:
      motor_output (motorA, LOW, speed);
      motor_output (motorB, HIGH, -1);    // -1: no PWM set
      break;
    case BRAKE:
      // The AdaFruit library didn't implement a brake.
      // The L293D motor driver ic doesn't have a good
      // brake anyway.
      // It uses transistors inside, and not mosfets.
      // Some use a software break, by using a short
      // reverse voltage.
      // This brake will try to brake, by enabling
      // the output and by pulling both outputs to ground.
      // But it isn't a good break.
      motor_output (motorA, LOW, 255); // 255: fully on.
      motor_output (motorB, LOW, -1);  // -1: no PWM set
      break;
    case RELEASE:
      motor_output (motorA, LOW, 0);  // 0: output floating.
      motor_output (motorB, LOW, -1); // -1: no PWM set
      break;
    default:
      break;
    }
  }
}


// ---------------------------------
// motor_output
//
// The function motor_ouput uses the motor driver to
// drive normal outputs like lights, relays, solenoids,
// DC motors (but not in reverse).
//
// It is also used as an internal helper function
// for the motor() function.
//
// The high_low variable should be set 'HIGH'
// to drive lights, etc.
// It can be set 'LOW', to switch it off,
// but also a 'speed' of 0 will switch it off.
//
// The 'speed' sets the PWM for 0...255, and is for
// both pins of the motor output.
//   For example, if motor 3 side 'A' is used to for a
//   dimmed light at 50% (speed is 128), also the
//   motor 3 side 'B' output will be dimmed for 50%.
// Set to 0 for completelty off (high impedance).
// Set to 255 for fully on.
// Special settings for the PWM speed:
//    Set to -1 for not setting the PWM at all.
//
void motor_output (int output, int high_low, int speed)
{
  int motorPWM;

  switch (output)
  {
  case MOTOR1_A:
  case MOTOR1_B:
    motorPWM = MOTOR1_PWM;
    break;
  case MOTOR2_A:
  case MOTOR2_B:
    motorPWM = MOTOR2_PWM;
    break;
  case MOTOR3_A:
  case MOTOR3_B:
    motorPWM = MOTOR3_PWM;
    break;
  case MOTOR4_A:
  case MOTOR4_B:
    motorPWM = MOTOR4_PWM;
    break;
  default:
    // Use speed as error flag, -3333 = invalid output.
    speed = -3333;
    break;
  }

  if (speed != -3333)
  {
    // Set the direction with the shift register
    // on the MotorShield, even if the speed = -1.
    // In that case the direction will be set, but
    // not the PWM.
    shiftWrite(output, high_low);

    // set PWM only if it is valid
    if (speed >= 0 && speed <= 255)    
    {
      analogWrite(motorPWM, speed);
    }
  }
}


// ---------------------------------
// shiftWrite
//
// The parameters are just like digitalWrite().
//
// The output is the pin 0...7 (the pin behind
// the shift register).
// The second parameter is HIGH or LOW.
//
// There is no initialization function.
// Initialization is automatically done at the first
// time it is used.
//
void shiftWrite(int output, int high_low)
{
  static int latch_copy;
  static int shift_register_initialized = false;

  // Do the initialization on the fly,
  // at the first time it is used.
  if (!shift_register_initialized)
  {
    // Set pins for shift register to output
    pinMode(MOTORLATCH, OUTPUT);
    pinMode(MOTORENABLE, OUTPUT);
    pinMode(MOTORDATA, OUTPUT);
    pinMode(MOTORCLK, OUTPUT);

    // Set pins for shift register to default value (low);
    digitalWrite(MOTORDATA, LOW);
    digitalWrite(MOTORLATCH, LOW);
    digitalWrite(MOTORCLK, LOW);
    // Enable the shift register, set Enable pin Low.
    digitalWrite(MOTORENABLE, LOW);

    // start with all outputs (of the shift register) low
    latch_copy = 0;

    shift_register_initialized = true;
  }

  // The defines HIGH and LOW are 1 and 0.
  // So this is valid.
  bitWrite(latch_copy, output, high_low);

  // Use the default Arduino 'shiftOut()' function to
  // shift the bits with the MOTORCLK as clock pulse.
  // The 74HC595 shiftregister wants the MSB first.
  // After that, generate a latch pulse with MOTORLATCH.
  shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(MOTORLATCH, HIGH);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(MOTORLATCH, LOW);
}
