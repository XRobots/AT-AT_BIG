// ***** LEG 2 ****
int leg1z;
int leg1za;
int leg1x;
int leg1zScaled;
int leg1xScaled;
int check;
int homeFlag = 0;
int goFlag = 0;
int topFlag = 0;
int bottomFlag = 0;

//PID
#include <PID_v1.h>

// PID1
double Pk1 = 3; 
double Ik1 = 0;
double Dk1 = 0;

double Setpoint1, Input1, Output1, Output1a;    // PID variables
PID PID1(&Input1, &Output1, &Setpoint1, Pk1, Ik1 , Dk1, DIRECT);    // PID Setup

// PID2
double Pk2 = 3; 
double Ik2 = 0;
double Dk2 = 0;

double Setpoint2, Input2, Output2, Output2a;    // PID variables
PID PID2(&Input2, &Output2, &Setpoint2, Pk2, Ik2 , Dk2, DIRECT);    // PID Setup

unsigned long currentMillis;
unsigned long previousMillis = 0;        // set up timers
long interval = 10;

int button1;
int button2;
int switch1;
int potTop;
int potBottom;

void setup() {
  
  Serial.begin(115200);   // debug
  Serial1.begin(115200);  // leg data
  
  pinMode(13, OUTPUT);        // diags LED
  pinMode(2, INPUT_PULLUP);   // RH button
  pinMode(3, INPUT_PULLUP);   // LH button
  pinMode(4, INPUT_PULLUP);   // switch
  
  pinMode(A0, INPUT);         // upper leg pot
  pinMode(A1, INPUT);         // lower leg pot
  
  pinMode(18, OUTPUT);        // upper leg PWM
  pinMode(19, OUTPUT);
  pinMode(22, OUTPUT);        // lower leg PWM
  pinMode(23, OUTPUT);
  
  PID1.SetMode(AUTOMATIC);              
  PID1.SetOutputLimits(-127,127);
  PID1.SetSampleTime(10);
    
  PID2.SetMode(AUTOMATIC);              
  PID2.SetOutputLimits(-127,127);
  PID2.SetSampleTime(10);
}

void loop() {

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
  
  if (Serial1.available() > 1){           // receive IMU data from serial
        digitalWrite(13, HIGH);           // serial data is there
        check = Serial1.parseInt();
        if (check == 1000) {                   // look for check value to check it's the start of the data
            leg1z = Serial1.parseInt();
            leg1x = Serial1.parseInt();
            if (Serial1.read() == '\n') {     // end of data             
            }
          }
        }
  else digitalWrite(13, LOW);    // no serial data

  leg1xScaled = map(leg1x, -400,400,190,-190);      // scale incoming data for each axis
  leg1xScaled = constrain(leg1xScaled,-190,190);
  leg1za = leg1z - 28;    //trim
  leg1zScaled = map(leg1za, -400,135,1165,1305);
  leg1zScaled = constrain(leg1zScaled,1165,1305);  
  
  button1 = digitalRead(2);    // lower leg home
  button2 = digitalRead(3);    // upper leg home
  switch1 = digitalRead(4);    // switch1 
  potTop = analogRead(A0);     // top pot  
  potBottom = analogRead(A1);     // top pot

  potTop = potTop - 512;
  potBottom = potBottom - 512;

  /*

  Serial.print(potTop);         //+ve +430
  Serial.print(" , ");
  Serial.print(potBottom);         //-ve -430
  Serial.print(" , ");
  Serial.print(goFlag); 
  Serial.print(" , ");
  Serial.print(topFlag);
  Serial.print(" , ");
  Serial.println(bottomFlag);

  */

  // homing buttons

  // ***TOP***
  
  if (button1 == 0 && topFlag == 0) {
    topFlag = 1;
  }
  else if (topFlag == 1) {
    if (potTop > -400 && switch1 == 0) {  
      analogWrite(19, 0);
      analogWrite(18, 50);
    }
    else {
      analogWrite(19, 0);
      analogWrite(18, 0);
      topFlag = 2;
    }
  }    
  else if (topFlag == 2 && switch1 == 0) {
    if (potTop < -265) {
      analogWrite(19, 50);
      analogWrite(18, 0);
    }
    else {
      analogWrite(19, 0);
      analogWrite(18, 0);
      topFlag = 0;
    }
  }
  // ***BOTTOM***

  if (button2 == 0 && bottomFlag == 0) {
    bottomFlag = 1;
  }
  else if (bottomFlag == 1) {
    if (potBottom <= 400 && switch1 == 0) {  
      analogWrite(22, 0);
      analogWrite(23, 50);
    }
    else {
      analogWrite(22, 0);
      analogWrite(23, 0);
      bottomFlag = 2;
    }
  }   
  else if (bottomFlag == 2 && switch1 == 0) {
    if (potBottom > 265) {
      analogWrite(22, 50);
      analogWrite(23, 0);
    }
    else {
      analogWrite(22, 0);
      analogWrite(23, 0);
      bottomFlag = 0;
    }
  }

    // process incoming data under normal state
  
    if (potTop < -250 && potTop > -280 && potBottom > 250 && potBottom < 280 && goFlag == 0) {
      goFlag = 1;
    }

    if (goFlag == 1 && topFlag == 0 && bottomFlag == 0) {
      kinematics(leg1xScaled, leg1zScaled);     // run inverse kinematics and drive joints
    }

  
  }  // end of timed loop

} // end of main loop
