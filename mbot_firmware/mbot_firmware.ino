

#include <SoftwareSerial.h>
#include "MeOrion.h"

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
//SoftwareSerial Xbee(10, 9); // RX, TX

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
}

void loop()
{
 while(Serial.available() > 0){
        if(index == -1){         // looking for new packet
            if(Serial.read() == 0xff){
                index = 0;
                checksum = 0;
            }
        }else if(index == 0){
            vals[index] = (unsigned char) Serial.read();
            if(vals[index] != 0xff){
                checksum += (int) vals[index];
                index++;
            }
        }else{
            vals[index] = (unsigned char) Serial.read();
            checksum += (int) vals[index];
            index++;
            if(index == 7){ // packet complete    
                if(checksum%256 != 255){
                    index = -1;
                }
                
                
            else{
                        joysticRV = (signed char)( (int)vals[0]-161 );
                        joysticRH = (signed char)( (int)vals[1]-94 );
                        joysticLV = (signed char)( (int)vals[2]-160 );
                        joysticLH = (signed char)( (int)vals[3]-97 );
                    buttons = vals[4];
                }
                index = -1;
                Serial.flush();
            }
             
        }
    }
    int cmdLeft = joysticLV * 3;
    int cmdRight = joysticRV * 3;

    motor1.run(cmdLeft); /* value: between -255 and 255. */
    motor2.run(cmdRight); /* value: between -255 and 255. */
   
    Serial.println(cmdLeft); // Data to send on Xbee chanel 
    delay(30);
}
