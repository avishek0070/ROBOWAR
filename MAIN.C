#include <IBusBM.h>
#include <Servo.h>
#include<CytronMotorDriver.h>

int comp = 0;
float turn =0.25;
unsigned long spindelay = 100; //calibrate with bot

Servo ESC;     // create servo object to control the ESC


CytronMD motor1(PWM_DIR, 3, 4);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor2(PWM_DIR, 9, 10); // PWM 2 = Pin 9, DIR 2 = Pin 10.



//IBusBM IBus; // IBus object
IBusBM IBusServo;
//IBusBM IBusSensor;
//#define TEMPBASE 400    // base value for 0'C

// sensor values
//uint16_t speed=0;
//uint16_t temp=TEMPBASE+200; // start at 20'C

  
void setup() {
  int motora,motorb,wspeed;
  int ch1,ch2,ch3,ch4,ch5,ch6;

  Serial.begin(115200);   // remove comment from this line if you change the Serial port in the next line

//IBus.begin(Serial1);    // iBUS connected to Serial0 - change to Serial1 or Serial2 port when required
IBusServo.begin(Serial1);
//IBusSensor.begin(Serial2);

  Serial.println("Start iBUS monitor");

  // adding 2 sensors
  //IBusSensor.addSensor(IBUSS_RPM);
  // IBusSensor.addSensor(IBUSS_TEMP);
  

  ESC.attach(7,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 

  Serial.println("Start IBus2PWM");
  Serial.println("Wait for receiver");
  while(IBusServo.cnt_rec==0) delay(100);//waits for txrx connection to complete
 // ch1 = map(IBusServo.readChannel(1),1000,2000,-255,255); // index 1 is vertical channel actually ch2 in FS-I6
 // ch2  = map(IBusServo.readChannel(0),1000,2000,-255,255); // index 0 is horizontal channel actually ch1 in FS-I6
  //ch3comp = map(IBusServo.readChannel(2),1000,2000,0,180);
  Serial.println("Init done");

  motora=0;
  motorb=0;
  wspeed = 0;

  ch1=0;
  ch2=0;
  ch3=0;
 
  //ch3comp=0;
  ESC.write(wspeed);
  motor1.setSpeed(0);
  motor2.setSpeed(0);


}


void loop() {
int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  ch1 = map(IBusServo.readChannel(1),1000,2000,-255,255); //read value from servo channel2 aka vertical channel
  ch2 = map(IBusServo.readChannel(0),1000,2000,-255,255); // read value from servo vhannel1 aka horizontal channel
  ch3= map(IBusServo.readChannel(2),1000,2000,0,180); //
  ch4=map(IBusServo.readChannel(3),1000,2000,-255,255);
  ch5=IBusServo.readChannel(4);
  ch6=IBusServo.readChannel(5);


  for (int i=0; i<6 ; i++) {
    Serial.print(IBusServo.readChannel(i));
    Serial.print(" ");
  }
  Serial.print("Cnt=");
  Serial.print(IBusServo.cnt_rec); // count of how many times servo values have been updated
  //Serial.print(" POLL=");
  //Serial.print(IBusSensor.cnt_poll); // count of polling for sensor existance
 // Serial.print(" Sensor=");
 // Serial.print(IBusSensor.cnt_sensor); // count of polling for sensor value
  Serial.print(" ch1 = ");
  Serial.print(ch1);
  Serial.print("  ch2= ");
  Serial.print(ch2);
  Serial.print("  ch4 = ");
  Serial.println(ch4);
  
  //IBusSensor.setSensorMeasurement(1,speed);
  //speed += 10;                           // increase motor speed by 10 RPM
  //IBusSensor.setSensorMeasurement(2,temp++); // increase temperature by 0.1 'C every loop

  

  if(ch6<1500){ //normal xy control main set, swb pos is up


    if(ch5 > 1500){  //normal x and y control, swc position is down
          if (ch1>comp){ //check if joystick is pointiog foward then determine direction to go
            if(ch2==comp){ // nested if working better forward command set
              moveforward(); 
              Serial.println("Moveing Forward");
            }else if(ch2>comp){
              moveforwardright();
              Serial.println("Moveing Forward right");
            }else if(ch2<comp){
              moveforwardleft();
              Serial.println("Moveing Forward left");
            }
          }else if(ch1==comp){ //roatation and maintain position command set
            if(ch2 > comp){
              moveright();
              Serial.println("Moveing right");
            }else if(ch2 < comp){
              moveleft();
              Serial.println("Moveing left");
            }else if( ch2==comp){
              maintainpos();
              Serial.println("Maintain pos");
            }
            
          }else if(ch1<comp){ // reverse command set
            if(ch2 == comp){
              moveback();
              Serial.println("Moving back");
            }else if(ch2 > comp){
              movebackright();
              Serial.println("Moveing back right");
            }else if(ch2 < comp){
              movebackleft();
              Serial.println("Moveing back left");
            }
              }
    } //normal xy control set end

  }else if (ch6>1500){ //invert both xy


      if (ch1<comp){ //check if joystick is pointiog foward then determine direction to go
                if(ch2==comp){ // nested if working better 
                  moveforward(); 
                  Serial.println("Moveing Forward");
                }else if(ch2<comp){
                  moveforwardright();
                  Serial.println("Moveing Forward right");
                }else if(ch2>comp){
                  moveforwardleft();
                  Serial.println("Moveing Forward left");
                }
              }else if(ch1==comp){
                if(ch2 < comp){
                  moveright();
                  Serial.println("Moveing right");
                }else if(ch2 > comp){
                  moveleft();
                  Serial.println("Moveing left");
                }else if( ch2==comp){
                  maintainpos();
                  Serial.println("Maintain pos");
                }
                
              }else if(ch1>comp){
                if(ch2 == comp){
                  moveback();
                  Serial.println("Moving back");
                }else if(ch2 < comp){
                  movebackright();
                  Serial.println("Moveing back right");
                }else if(ch2 > comp){
                  movebackleft();
                  Serial.println("Moveing back left");
                }
                  }// xy invert command set end
  }// main move command set end

  
  if(ch4>comp){
    right180();
    Serial.println("right180");
  }
  else if(ch4<comp){
    left180();
    Serial.println("Left180");
  } 

  
  weaponspeed();
  if (ch5<1250){
    KILL();
  }


  
  delay(600);//remove for comp currently used for debugging
  
}


void moveforward(){
int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  ch1 = map(IBusServo.readChannel(1),1000,2000,-255,255);
  ch2 = map(IBusServo.readChannel(0),1000,2000,-255,255);
  
    motora=ch1;
    motorb=ch1;

    Serial.print("MotorA = "); //debugging
    Serial.print(motora);
    Serial.print("MotorB = ");
    Serial.println(motorb);
  
  

  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);
  Serial.print("motorA");
  Serial.println(ch1);

}
void moveforwardright(){
int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  ch1 = map(IBusServo.readChannel(1),1000,2000,-255,255);
  ch2 = map(IBusServo.readChannel(0),1500,2000,0,100);
  
  motora=ch1 - (int(turn*(ch1*(ch2/100))));
  motorb=ch1 + (int((ch1*(ch2/100)))); //to change turn characteristic by small increment (for motorb to run) value change percentage by subtracting small amount from ch2   
  Serial.print("MotorA = "); // debugging 
  Serial.print(motora);
  Serial.print("MotorB = ");
  Serial.println(motorb);
  

  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);

}
void moveforwardleft(){
  float ch1,ch2,ch3,ch4,ch5,ch6;
  float motora,motorb,wspeed,turn1,turn2;
  ch1 = map(IBusServo.readChannel(1),1000,2000,-255,255);
  ch2 = map(IBusServo.readChannel(0),1000,1500,1,255);
   
  turn1 =((turn*(ch1*(ch2/255))));
  turn2 = (((ch1*(ch2/255))));
  motorb=ch1 - turn1 ;
  motora=ch1 + turn2 ; //to change turn characteristic by small increment (for motorb to run) value change percentage by subtracting small amount from ch2  
  Serial.print("MotorA = "); // debugging 
  Serial.print(motora);
  Serial.print("MotorB = ");
  Serial.println(motorb);
  Serial.print("turn1 = "); // debugging 
  Serial.print(turn1);
  Serial.print("turn2= ");
  Serial.println(turn2);

  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);

}
void moveright(){
  int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  ch1 = map(IBusServo.readChannel(1),1000,2000,-255,255);
  ch2 = map(IBusServo.readChannel(0),1000,2000,-255,255);

    motora=(0-ch2);
    motorb=ch2;
 

  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);


}
void moveleft(){
  int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  ch1 = map(IBusServo.readChannel(1),1000,2000,-255,255);
  ch2 = map(IBusServo.readChannel(0),1000,2000,-255,255);
  
    motora=ch2;
    motorb=(0-ch2);


  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);


}
void moveback(){
  int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  ch1 = map(IBusServo.readChannel(1),1000,2000,-255,255);
  ch2 = map(IBusServo.readChannel(0),1000,2000,-255,255);
  
  motora=ch1;
  motorb=ch1;
  Serial.println(motora);


  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);

}
void movebackright(){
int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  ch1 = map(IBusServo.readChannel(1),1000,2000,-255,255);
  ch2 = map(IBusServo.readChannel(0),1500,2000,0,100);

  motora=ch1 - (int(turn*(ch1*(ch2/100))));
  motorb=ch1 + (int((ch1*(ch2/100))));


  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);

}

void movebackleft(){
  int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  ch1 = map(IBusServo.readChannel(1),1000,2000,-255,255);
  ch2 = map(IBusServo.readChannel(0),1000,1500,0,1);

  ch1= -ch1;
  motora = ch1 + (int(ch1 * ch2));
  motorb = ch1 - (int(turn * (ch1 * ch2)));
  


  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);

}
void maintainpos(){
  int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  motora=0;
  motorb=0;

  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);

}
void weaponspeed(){
  int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  ch3= map(IBusServo.readChannel(3),1000,2000,0,180);
  wspeed = ch3;

  ESC.write(wspeed);

}
void right180(){
  int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  motora = -255;
  motorb = 255;
  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);


  
}
void left180(){
  int ch1,ch2,ch3,ch4,ch5,ch6; // input values
int motora,motorb,wspeed;
  motora = 255;
  motorb = -255;
  motor1.setSpeed(motora);
  motor2.setSpeed(motorb);

 

}
void KILL(){
  int ch5; // input values
  int motora,motorb,wspeed;
  ch5=IBusServo.readChannel(4);
  if(ch5<1250){
     motor1.setSpeed(0);
    motor2.setSpeed(0);
    ESC.write(0);
  }

}
