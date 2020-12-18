// Include the required Wire library for I2C<br>
#include<Wire.h>
int a = 1;
void setup() {
  // Start the I2C Bus as Master
  Wire.begin();
}
void loop() {
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(a);              // sends a
  Wire.endTransmission();    // stop transmitting
  a++; // Increment a
 // if (a > 5) a = 0; // `reset a once it gets 6
  delay(500);
}
