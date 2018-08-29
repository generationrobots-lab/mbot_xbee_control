//Mbot code
/*
   _____ _____  _               ____  
  / ____|  __ \| |        /\   |  _ \ 
 | |  __| |__) | |       /  \  | |_) |
 | | |_ |  _  /| |      / /\ \ |  _ < 
 | |__| | | \ \| |____ / ____ \| |_) |
  \_____|_|  \_\______/_/    \_\____/

  */
#include <SoftwareSerial.h>
#include "MeOrion.h"

SoftwareSerial Xbee(10, 9); // RX, TX

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

unsigned char vals[7];  // temporary values, moved after we confirm checksum
int index;              // -1 = waiting for new packet
int checksum;
unsigned char status;


void setup() {
  Serial.begin(38400);
  Xbee.begin(38400);
}

void loop() {
  if (commanderRead()) {
    int speedCmd , angularCmd, rightMotorCmd, leftMotorCmd;
    speedCmd = map(vals[0], 20, 80, -126, 126);
    angularCmd = map(vals[1], 20, 80, -126, 126);
    rightMotorCmd = speedCmd - angularCmd;
    leftMotorCmd = speedCmd + angularCmd;
    Serial.print(rightMotorCmd);
    Serial.print(" - ");
    Serial.println(leftMotorCmd);
    motor1.run(leftMotorCmd); /* value: between -255 and 255. */
    motor2.run(rightMotorCmd); /* value: between -255 and 255. */
  }


}

bool commanderRead()
{
  //extract of arborix code.

  // Frame used : 8 bits, only 2 are used here, the first and the second.
  while (Xbee.available() > 0) {
    if (index == -1) {       // looking for new packet
      if (Xbee.read() == 0xff) {
        index = 0;
        checksum = 0;
      }
    } else if (index == 0) {
      vals[index] = (unsigned char) Xbee.read();
      if (vals[index] != 0xff) {
        checksum += (int) vals[index];
        index++;
      }
    } else {
      vals[index] = (unsigned char) Xbee.read();
      checksum += (int) vals[index];
      index++;
      if (index == 7) { // packet complete
        if (checksum % 256 != 255) {
          index = -1;
        }

        index = -1;
        Xbee.flush();

        return 1;
      }

    }
  }
  return 0;
}
