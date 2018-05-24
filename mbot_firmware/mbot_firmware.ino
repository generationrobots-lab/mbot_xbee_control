

#include <SoftwareSerial.h>
#include "MeOrion.h"

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
SoftwareSerial Xbee(10, 9); // RX, TX

unsigned char vals[7];  // temporary values, moved after we confirm checksum
int index;              // -1 = waiting for new packet
int checksum;
unsigned char status; 
signed char joysticLV;      // vertical stick movement = forward speed
signed char joysticLH;      // horizontal stick movement = sideways or angular speed
signed char joysticRV;      // vertical stick movement = tilt    
signed char joysticRH;      // horizontal stick movement = pan (when we run out of pan, turn body?)
unsigned char buttons;  // 

    
void setup()
{
  Serial.begin(38400);
  Xbee.begin(38400);
}

void loop()
{
  if(commanderRead()){

    // To customize 
    // Here is jsut to control motors and send datas 


      
      int cmdLeft = joysticLV * 3; //basic proportional calculation. Can be improved
      int cmdRight = joysticRV * 3; //basic proportional calculation. Can be improved


      // send commands to motors
      motor1.run(cmdLeft); /* value: between -255 and 255. */
      motor2.run(cmdRight); /* value: between -255 and 255. */

      // send commands to pc
      Xbee.print(cmdLeft); // Data to send on Xbee chanel 
      Xbee.print(";");
      Xbee.println(cmdRight);
  }
  delay(20);
}

bool commanderRead()
  {
    while(Xbee.available() > 0){
        if(index == -1){         // looking for new packet
            if(Xbee.read() == 0xff){
                index = 0;
                checksum = 0;
            }
        }else if(index == 0){
            vals[index] = (unsigned char) Xbee.read();
            if(vals[index] != 0xff){
                checksum += (int) vals[index];
                index++;
            }
        }else{
            vals[index] = (unsigned char) Xbee.read();
            checksum += (int) vals[index];
            index++;
            if(index == 7){ // packet complete    
                if(checksum%256 != 255){
                    index = -1;
                }
                
                
            else{
                        joysticRV = (signed char)( (int)vals[0]-161 ); // 161 => offset
                        joysticRH = (signed char)( (int)vals[1]-94 );// 94 => offset
                        joysticLV = (signed char)( (int)vals[2]-160 );// 160 => offset
                        joysticLH = (signed char)( (int)vals[3]-97 );// 97 => offset
                    buttons = vals[4];
                }
                index = -1;
                Xbee.flush();
                return 1;
            }
             
        }
    }
    return 0;
  }
