//Arduino code
/*
    _____ _____  _               ____
   / ____|  __ \| |        /\   |  _ \
  | |  __| |__) | |       /  \  | |_) |
  | | |_ |  _  /| |      / /\ \ |  _ <
  | |__| | | \ \| |____ / ____ \| |_) |
   \_____|_|  \_\______/_/    \_\____/

*/

#include <SoftwareSerial.h>

const byte joystickX = A0;
const byte joystickY = A1;
unsigned char joystickXValue, joystickYValue;

SoftwareSerial Xbee(10, 9); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Xbee.begin(38400);
}

void loop() {

  // FRAME TO SEND :
  Xbee.write(0xff); //new packet First bit
  joystickXValue = analogRead(joystickX) / 10 ; //
  joystickYValue = analogRead(joystickY) / 10 ; //
  Xbee.write(joystickXValue); //Second bit
  Xbee.write(joystickYValue);// third bit
  Xbee.write((unsigned char)0);   //(unused) fourth bit
  Xbee.write((unsigned char)0);   //(unused) fifht bit
  Xbee.write((unsigned char)0);   //(unused) sixth bit
  Xbee.write((unsigned char)0);   //(unused) seventh bit
  Xbee.write((unsigned char)(255 - (joystickXValue + joystickYValue) % 256)); //check value
  delay(100);
}


