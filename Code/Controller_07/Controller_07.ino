// ramp lib
#include <Ramp.h>

// #include <FastGPIO.h>
// #define APA102_USE_FAST_GPIO

#include <APA102.h>

// Define which pins to use.
const uint8_t dataPin = 28;
const uint8_t clockPin = 29;

int LEDon = 120;    // LED brightness
int LEDoff = 0;

// Create an object for writing to the LED strip.
APA102<dataPin, clockPin> ledStrip;

// Set the number of LEDs to control.
const uint16_t ledCount = 4;

// encoder setup

#define encoder0PinA 33
#define encoder0PinB 34

volatile long encoder0Pos = 0;
int encoder0PosScaled;

// clock setup

unsigned long currentMillis;
unsigned long previousMillis = 0;        // set up timers
unsigned long previousZeroMillis = 0;
long interval = 20;                      // time constant for timer

// switches

int sw1;
int sw2;
int sw3;
int sw4;
int sw5;
int sw6;
int sw7;
int sw8;
int sw9;
int sw10;
int sw11;

// leg coordinates
int leg1z;
int leg1x;
int leg2z;
int leg2x;
int leg3z;
int leg3x;
int leg4z;
int leg4x;

int leg1zPos;
int leg1xPos;
int leg2zPos;
int leg2xPos;
int leg3zPos;
int leg3xPos;
int leg4zPos;
int leg4xPos;

int stride;

int walkXPos1;
int walkXPos2;
int walkXPos3; 
int walkXPos4; 
int walkXPos5;
int walkXPos6; 
int walkXPos7; 
int walkXPos8;

int legDown;
int legUp;

int stepFlag = 0;
int walkFlag = 0;
int turnFlag = 0;
long previousStepMillis = 0;    // timers for walking states 
long previousTurnMillis = 0;    // timers for walking states 
int timer1;
int timer2;
int dur1;
int turn;



class Interpolation {
  public:
    rampInt myRamp;
    int interpolationFlag = 0;
    int savedValue;

    int go(int input, int duration) {

      if (input != savedValue) {   // check for new data
        interpolationFlag = 0;
      }
      savedValue = input;          // bookmark the old value

      if (interpolationFlag == 0) {                                   // only do it once until the flag is reset
        myRamp.go(input, duration, LINEAR, ONCEFORWARD);              // start interpolation (value to go to, duration)
        interpolationFlag = 1;
      }

      int output = myRamp.update();
      return output;
    }
};    // end of class

Interpolation interpFLX;
Interpolation interpFLZ;

Interpolation interpFRX;
Interpolation interpFRZ;

Interpolation interpBLX;
Interpolation interpBLZ;

Interpolation interpBRX;
Interpolation interpBRZ;

void setup() {

  Serial.begin (115200);      // debug
  Serial1.begin (115200);     // legs
  Serial2.begin (115200);
  Serial3.begin (115200);
  Serial4.begin (115200);
  Serial5.begin (115200);     // linear actuator

// encoders
  pinMode(encoder0PinA, INPUT_PULLUP); 
  pinMode(encoder0PinB, INPUT_PULLUP); 
// encoder pin on interrupt
  attachInterrupt(33, doEncoderA, CHANGE);
  attachInterrupt(34, doEncoderB, CHANGE);

// 5x push switches
pinMode(2, INPUT_PULLUP); 
pinMode(3, INPUT_PULLUP); 
pinMode(4, INPUT_PULLUP); 
pinMode(5, INPUT_PULLUP); 
pinMode(6, INPUT_PULLUP);

// 6x toggles 
pinMode(9, INPUT_PULLUP); 
pinMode(10, INPUT_PULLUP); 
pinMode(11, INPUT_PULLUP); 
pinMode(12, INPUT_PULLUP); 
pinMode(24, INPUT_PULLUP);
pinMode(25, INPUT_PULLUP); 
 
}

void loop(){

  // 5x push switches 
  sw1 = digitalRead(5);
  sw2 = digitalRead(4);
  sw3 = digitalRead(2);
  sw4 = digitalRead(3);
  sw5 = digitalRead(6);

  // 6x toggles  
  sw6 = digitalRead(12);
  sw7 = digitalRead(10);
  sw8 = digitalRead(9);
  sw9 = digitalRead(11);
  sw10 = digitalRead(25);
  sw11 = digitalRead(24);

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // scale and constrain encoder;
        if (encoder0Pos >= 2000) {
          encoder0Pos = 2000;
        }
        if (encoder0Pos <= -2000) {
          encoder0Pos = -2000;
        }
        encoder0PosScaled = encoder0Pos/5;
        encoder0PosScaled = constrain(encoder0PosScaled,-400,400);

        // zero switch
        if (sw5 == 0) {
          encoder0Pos = 0;
        }

        // LED handling       
        if (encoder0PosScaled == 0) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDon, LEDoff, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.endFrame(4);          
        }        
        else if (encoder0PosScaled > 0 && encoder0PosScaled <=100) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.endFrame(4);          
        }
        else if (encoder0PosScaled >=101 && encoder0PosScaled <=200) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.endFrame(4);          
        }
        else if (encoder0PosScaled >=201 && encoder0PosScaled <=300) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.endFrame(4);          
        }
        else if (encoder0PosScaled >=301 && encoder0PosScaled <=389) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.endFrame(4);          
        }
        else if (encoder0PosScaled >=390) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDoff, LEDon, LEDoff);
            ledStrip.sendColor(LEDon, LEDoff, LEDoff);
            ledStrip.endFrame(4);          
        }
        else if (encoder0PosScaled < 0 && encoder0PosScaled >= -100) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.endFrame(4);          
        }
        else if (encoder0PosScaled <= 101 && encoder0PosScaled >= -200) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.endFrame(4);          
        }
        else if (encoder0PosScaled <= 201 && encoder0PosScaled >= -300) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDoff);
            ledStrip.endFrame(4);          
        }
        else if (encoder0PosScaled <= 301 && encoder0PosScaled >= -390) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.endFrame(4);          
        }
        else if (encoder0PosScaled <= 390) {
            ledStrip.startFrame();
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDoff, LEDoff, LEDon);
            ledStrip.sendColor(LEDon, LEDoff, LEDoff);
            ledStrip.endFrame(4);          
        }

        // *** jog legs ***

        if (sw10 == 0 && sw11 == 0 && stepFlag == 0 && turnFlag == 0) {

            dur1 = 0;   // interpolate over nothing

            if (sw1 == 0) {       // enable leg 1
                if (sw6 == 0) {   // z axis
                  leg1zPos = encoder0PosScaled;
                }
                else if (sw6 == 1) {
                  leg1xPos = encoder0PosScaled;
                }
            }
    
            if (sw2 == 0) {       // enable leg 2
                if (sw7 == 0) {   // z axis
                  leg2zPos = encoder0PosScaled;
                }
                else if (sw7 == 1) {
                  leg2xPos = encoder0PosScaled;
                }
            }
    
            if (sw3 == 0) {       // enable leg 3
                if (sw8 == 0) {   // z axis
                  leg3zPos = encoder0PosScaled;
                }
                else if (sw8 == 1) {
                  leg3xPos = encoder0PosScaled;
                }
            }
    
            if (sw4 == 0) {       // enable leg 4
                if (sw9 == 0) {   // z axis
                  leg4zPos = encoder0PosScaled;
                }
                else if (sw9 == 1) {
                  leg4xPos = encoder0PosScaled;
                }
            }

        } // end of jog legs

        if (sw10 == 1 && sw11 == 0) {
            walkFlag = 1;
        }
        else if (sw11 == 1 && sw10 == 0) {
            walkFlag = 2;
        }
        else if (sw10 == 0 && sw11 == 0) {
            walkFlag = 0;
            if (encoder0PosScaled > 200) {          // set direction or nothing based on encoder value    
                stride = 220;
                turn = 0;
            }
            else if (encoder0PosScaled == 0) {
                stride = 0;
                turn = 1;
            }
            else if (encoder0PosScaled < -200) {
                stride = -220;
                turn = 1;
            }
        }

        Serial.println(turn);

        //stride = 180;

        walkXPos1 = stride * 0.75;
        walkXPos2 = stride * 0;
        walkXPos3 = stride * -0.75; 
        walkXPos4 = stride * -0.5; 
        walkXPos5 = stride * -0.25;
        walkXPos6 = stride * 0; 
        walkXPos7 = stride * 0.25; 
        walkXPos8 = stride * 0.5;

        legDown = 0;    // leg down
        legUp = -400;    // leg up        

        // **********************************
        // *** get feet ready for walking ***
        // **********************************

        if (stepFlag == 0 && walkFlag == 1 && currentMillis - previousStepMillis > timer1) {
          timer1 = 1200;
          dur1 = 1000;
          previousStepMillis = currentMillis;
          stepFlag = 1;
          leg1xPos = walkXPos1;   // +75
          leg1zPos = legUp;       // leg Up
          leg2xPos = walkXPos2;   // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos2;   // 0
          leg3zPos = legDown;
          leg4xPos = walkXPos2;   // 0
          leg4zPos = legDown;        
        }
        else if (stepFlag == 1 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 2;
          leg1xPos = walkXPos1;   // +75
          leg1zPos = legDown;     // leg Down
          leg2xPos = walkXPos2;   // 0;
          leg2zPos = legDown;
          leg3xPos = walkXPos2;   // 0;
          leg3zPos = legDown;
          leg4xPos = walkXPos2;   // 0;  
          leg4zPos = legDown;
        }
        else if (stepFlag == 2 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 3;
          leg1xPos = walkXPos1;   // +75
          leg1zPos = legDown;
          leg2xPos = walkXPos2;   // 0;
          leg2zPos = legDown;
          leg3xPos = walkXPos7;   // +25
          leg3zPos = legUp;       // leg Up
          leg4xPos = walkXPos2;   // 0;
          leg4zPos = legDown;
        }
        else if (stepFlag == 3 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 4;
          leg1xPos = walkXPos1;   // +75
          leg1zPos = legDown;
          leg2xPos = walkXPos2;   // 0;
          leg2zPos = legDown;
          leg3xPos = walkXPos7;   // +25
          leg3zPos = legDown;     // leg Down
          leg4xPos = walkXPos2;   // 0;
          leg4zPos = legDown;
        }
        else if (stepFlag == 4 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 5;
          leg1xPos = walkXPos1;   // +75
          leg1zPos = legDown;
          leg2xPos = walkXPos5;   // -25
          leg2zPos = legUp;       // leg Up
          leg3xPos = walkXPos7;   // +25;
          leg3zPos = legDown;
          leg4xPos = walkXPos2;   // 0;
          leg4zPos = legDown;
        }
        else if (stepFlag == 5 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 6;
          leg1xPos = walkXPos1;   // +75
          leg1zPos = legDown;
          leg2xPos = walkXPos5;   // -25
          leg2zPos = legDown;     // leg Down
          leg3xPos = walkXPos7;   // +25;   
          leg3zPos = legDown;
          leg4xPos = walkXPos2;   // 0;
          leg4zPos = legDown;
        }
        else if (stepFlag == 6 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 7;
          leg1xPos = walkXPos1;   // +75
          leg1zPos = legDown;
          leg2xPos = walkXPos5;   // -25;
          leg2zPos = legDown;
          leg3xPos = walkXPos7;   // +25;
          leg3zPos = legDown;
          leg4xPos = walkXPos3;   // -75
          leg4zPos = legUp;       // leg Up
        }
        else if (stepFlag == 7 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          if (walkFlag == 1) {
              stepFlag = 8;             // carry on walking
          }
          else if (walkFlag == 0) {
              stepFlag = 91;            // go to end - put feet straight               
          }
          leg1xPos = walkXPos1;   // +75
          leg1zPos = legDown;
          leg2xPos = walkXPos5;   // -25;
          leg2zPos = legDown;
          leg3xPos = walkXPos7;   // +25;
          leg3zPos = legDown;
          leg4xPos = walkXPos3;   // -75
          leg4zPos = legDown;     // leg Down
        }        
        
        // *****************************
        // *** end of start sequence *** 
        // *****************************
        // *** start continuos walk ****
        // *****************************        
        
        else if (stepFlag == 8 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 9;
          leg1xPos = walkXPos8 ;    // +50
          leg1zPos = legDown;
          leg2xPos = walkXPos4;     // -50
          leg2zPos = legDown;
          leg3xPos = walkXPos6;     // 0
          leg3zPos = legDown;
          leg4xPos = walkXPos2;     // 0
          leg4zPos = legUp;         // Leg Up                
        }
        else if (stepFlag == 9 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 10;
          leg1xPos = walkXPos7;     // +25
          leg1zPos = legDown;
          leg2xPos = walkXPos3;     // -75
          leg2zPos = legDown;
          leg3xPos = walkXPos5;     // -25
          leg3zPos = legDown;
          leg4xPos = walkXPos1;     // +75
          leg4zPos = legDown;       // Leg Down                 
        }
        else if (stepFlag == 10 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 11;
          leg1xPos = walkXPos6;     // 0
          leg1zPos = legDown;
          leg2xPos = walkXPos2;     // 0
          leg2zPos = legUp;         // Leg Up
          leg3xPos = walkXPos4;     // -50
          leg3zPos = legDown;
          leg4xPos = walkXPos8;     // +50
          leg4zPos = legDown;                
        }
        else if (stepFlag == 11 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 12;
          leg1xPos = walkXPos5;     // -25
          leg1zPos = legDown;
          leg2xPos = walkXPos1;     // +75
          leg2zPos = legDown;       // Leg Down
          leg3xPos = walkXPos3;     // -75
          leg3zPos = legDown;
          leg4xPos = walkXPos7;     // +25
          leg4zPos = legDown;               
        }
        else if (stepFlag == 12 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 13;
          leg1xPos = walkXPos4;    // -50
          leg1zPos = legDown;
          leg2xPos = walkXPos8;    // +50
          leg2zPos = legDown;
          leg3xPos = walkXPos2;    // 0
          leg3zPos = legUp;        // Leg Up
          leg4xPos = walkXPos6;    // 0
          leg4zPos = legDown;                
        }
        else if (stepFlag == 13 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 14;
          leg1xPos = walkXPos3;     // -75
          leg1zPos = legDown;
          leg2xPos = walkXPos7;     // +25
          leg2zPos = legDown;
          leg3xPos = walkXPos1;     // +75
          leg3zPos = legDown;       // Leg Down
          leg4xPos = walkXPos5;     // -25
          leg4zPos = legDown;                
        }
        else if (stepFlag == 14 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 15;
          leg1xPos = walkXPos2;     // 0
          leg1zPos = legUp;         // Leg Up
          leg2xPos = walkXPos6;     // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos8;     // +50
          leg3zPos = legDown;
          leg4xPos = walkXPos4;     // -50
          leg4zPos = legDown;                
        }
        else if (stepFlag == 15 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          if (walkFlag == 1) {
              stepFlag = 8;             // carry on walking
          }
          else if (walkFlag == 0) {
              stepFlag = 91;            // go to end - put feet straight               
          }
          leg1xPos = walkXPos1;     // +75;
          leg1zPos = legDown;       // Leg Down
          leg2xPos = walkXPos5;     // -25
          leg2zPos = legDown;
          leg3xPos = walkXPos7;     // +25
          leg3zPos = legDown;
          leg4xPos = walkXPos3;     // -75
          leg4zPos = legDown;                
        }

        // *****************************
        // ** end of walking sequence ** 
        // *****************************
        // ***** start foot homing *****
        // ***************************** 

        else if (stepFlag == 91 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 92;
          leg1xPos = walkXPos1;     // +75;
          leg1zPos = legDown;
          leg2xPos = walkXPos5;     // -25
          leg2zPos = legDown;
          leg3xPos = walkXPos7;     // +25
          leg3zPos = legDown;
          leg4xPos = walkXPos3;     // -75
          leg4zPos = legDown;  
        }
        else if (stepFlag == 92 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 93;
          leg1xPos = walkXPos1;     // +75;
          leg1zPos = legDown;
          leg2xPos = walkXPos2;     // 0
          leg2zPos = legUp;         // Leg Up
          leg3xPos = walkXPos7;     // +25
          leg3zPos = legDown;
          leg4xPos = walkXPos3;     // -75
          leg4zPos = legDown;  
        }
        else if (stepFlag == 93 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 94;
          leg1xPos = walkXPos1;     // +75;
          leg1zPos = legDown;
          leg2xPos = walkXPos2;     // 0
          leg2zPos = legDown;       // Leg Down
          leg3xPos = walkXPos7;     // +25
          leg3zPos = legDown;
          leg4xPos = walkXPos3;     // -75
          leg4zPos = legDown;  
        }
        else if (stepFlag == 94 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 95;
          leg1xPos = walkXPos1;     // +75;
          leg1zPos = legDown;
          leg2xPos = walkXPos2;     // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos6;     // 0
          leg3zPos = legUp;         // Leg Up
          leg4xPos = walkXPos3;     // -75
          leg4zPos = legDown;  
        }
        else if (stepFlag == 95 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 96;
          leg1xPos = walkXPos1;     // +75;
          leg1zPos = legDown;
          leg2xPos = walkXPos2;     // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos6;     // 0
          leg3zPos = legDown;       // Leg Down
          leg4xPos = walkXPos3;     // -75
          leg4zPos = legDown;  
        }
        else if (stepFlag == 96 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 97;
          leg1xPos = walkXPos2;     // 0
          leg1zPos = legUp;         // Leg Up
          leg2xPos = walkXPos2;     // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos6;     // 0
          leg3zPos = legDown;
          leg4xPos = walkXPos3;     // -75
          leg4zPos = legDown;  
        }
        else if (stepFlag == 97 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 98;
          leg1xPos = walkXPos2;     // 0
          leg1zPos = legDown;       // Leg Down
          leg2xPos = walkXPos2;     // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos6;     // 0
          leg3zPos = legDown;
          leg4xPos = walkXPos3;     // -75
          leg4zPos = legDown;  
        }
        else if (stepFlag == 98 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 99;
          leg1xPos = walkXPos2;     // 0
          leg1zPos = legDown;
          leg2xPos = walkXPos2;     // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos6;     // 0
          leg3zPos = legDown;
          leg4xPos = walkXPos2;     // 0
          leg4zPos = legUp;         // Leg Up
        }
        else if (stepFlag == 99 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 0;
          leg1xPos = walkXPos2;     // 0;
          leg1zPos = legDown;
          leg2xPos = walkXPos2;     // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos6;     // 0
          leg3zPos = legDown;
          leg4xPos = walkXPos2;     // 0
          leg4zPos = legDown;       // Leg Down
        } 

        // *******************************
        // *** end of walking sequence ***
        // *******************************

        // *** start of turning sequence ***

        if (turnFlag == 0 && walkFlag == 2 && currentMillis - previousStepMillis > timer2) {
          timer2 = 1000;
          dur1 = 1000;
          previousStepMillis = currentMillis;
          turnFlag = 1;
          leg1xPos = walkXPos2;   // 0          
          leg2xPos = walkXPos2;   // 0          
          leg3xPos = walkXPos2;   // 0          
          leg4xPos = walkXPos2;   // 0
          if (turn == 1) {
            leg1zPos = legUp;
            leg2zPos = legDown;
            leg3zPos = legDown;
            leg4zPos = legUp;
          }
          else if (turn == 0) {
            leg1zPos = legDown;
            leg2zPos = legUp;
            leg3zPos = legUp;
            leg4zPos = legDown;
          }
          Serial5.print(500);
          Serial.println("Actuator stop");
          Serial2.print('\n');
        }
        if (turnFlag == 1 && currentMillis - previousStepMillis > timer2) {
          timer2 = 1000;
          dur1 = 1000;
          previousStepMillis = currentMillis;
          turnFlag = 2;
          leg1xPos = walkXPos2;   // 0
          leg2xPos = walkXPos2;   // 0
          leg3xPos = walkXPos2;   // 0
          leg4xPos = walkXPos2;   // 0
          if (turn == 1) {
            leg1zPos = legUp;
            leg2zPos = legDown;
            leg3zPos = legDown;
            leg4zPos = legUp;
          }
          else if (turn == 0) {
            leg1zPos = legDown;
            leg2zPos = legUp;
            leg3zPos = legUp;
            leg4zPos = legDown;
          }
          Serial5.print(600);
          Serial.println("Actuator open");
          Serial2.print('\n');      
        }
        if (turnFlag == 2 && currentMillis - previousStepMillis > 2000) {
          timer2 = 1000;
          dur1 = 1500;
          previousStepMillis = currentMillis;
          turnFlag = 3;
          leg1xPos = walkXPos2;   // 0
          leg1zPos = legDown;     // Legs Down
          leg2xPos = walkXPos2;   // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos2;   // 0
          leg3zPos = legDown;
          leg4xPos = walkXPos2;   // 0
          leg4zPos = legDown;
          Serial5.print(500);
          Serial.println("Actuator stop"); 
          Serial2.print('\n');       
        }
        if (turnFlag == 3 && currentMillis - previousStepMillis > timer2) {
          timer2 = 1000;
          dur1 = 1000;
          previousStepMillis = currentMillis;
          turnFlag = 4;
          leg1xPos = walkXPos2;   // 0
          leg1zPos = legDown;     // Legs Down
          leg2xPos = walkXPos2;   // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos2;   // 0
          leg3zPos = legDown;
          leg4xPos = walkXPos2;   // 0
          leg4zPos = legDown;  
          Serial5.print(500);
          Serial.println("Actuator stop");
          Serial2.print('\n');        
        }
        if (turnFlag == 4 && currentMillis - previousStepMillis > timer2) {
          timer2 = 1000;
          dur1 = 1000;
          previousStepMillis = currentMillis;
          turnFlag = 5;
          leg1xPos = walkXPos2;   // 0               
          leg2xPos = walkXPos2;   // 0          
          leg3xPos = walkXPos2;   // 0          
          leg4xPos = walkXPos2;   // 0
          if (turn == 1) {
            leg1zPos = legDown;
            leg2zPos = legUp;
            leg3zPos = legUp;
            leg4zPos = legDown;
          } 
          else if (turn == 0) {
            leg1zPos = legUp;
            leg2zPos = legDown;
            leg3zPos = legDown;
            leg4zPos = legUp;
          } 
          Serial5.print(500);
          Serial.println("Actuator stop");
          Serial2.print('\n');       
        }
        if (turnFlag == 5 && currentMillis - previousStepMillis > timer2) {
          timer2 = 1000;
          dur1 = 1000;
          previousStepMillis = currentMillis;
          turnFlag = 6;
          leg1xPos = walkXPos2;   // 0
          leg2xPos = walkXPos2;   // 0
          leg3xPos = walkXPos2;   // 0
          leg4xPos = walkXPos2;   // 0
          if (turn == 1) {
            leg1zPos = legDown;
            leg2zPos = legUp;
            leg3zPos = legUp;
            leg4zPos = legDown;
          } 
          else if (turn == 0) {
            leg1zPos = legUp;
            leg2zPos = legDown;
            leg3zPos = legDown;
            leg4zPos = legUp;
          } 
          Serial5.print(700);
          Serial.println("Actuator closed");
          Serial2.print('\n');        
        }
        if (turnFlag == 6 && currentMillis - previousStepMillis > 2000) {
          timer2 = 1000;
          dur1 = 1500;
          previousStepMillis = currentMillis;
          turnFlag = 7;
          leg1xPos = walkXPos2;   // 0
          leg1zPos = legDown;     // Legs Down
          leg2xPos = walkXPos2;   // 0
          leg2zPos = legDown;     
          leg3xPos = walkXPos2;   // 0
          leg3zPos = legDown;
          leg4xPos = walkXPos2;   // 0
          leg4zPos = legDown;  
          Serial5.print(700); 
          Serial.println("Actuator closed");
          Serial2.print('\n');    
        }
        if (turnFlag == 7 && currentMillis - previousStepMillis > timer2) {
          timer2 = 1000;
          dur1 = 1000;
          previousStepMillis = currentMillis;
          turnFlag = 0;
          leg1xPos = walkXPos2;   // 0
          leg1zPos = legDown;     // Legs Down
          leg2xPos = walkXPos2;   // 0
          leg2zPos = legDown;
          leg3xPos = walkXPos2;   // 0
          leg3zPos = legDown;
          leg4xPos = walkXPos2;   // 0
          leg4zPos = legDown;
          Serial5.print(700);   
          Serial.println("Actuator closed");
          Serial2.print('\n');    
        }
        
        // interpolate positions with ramp

        leg1x = interpFLX.go(leg1xPos,dur1);
        leg2x = interpFRX.go(leg2xPos,dur1);
        leg3x = interpBLX.go(leg3xPos,dur1);
        leg4x = interpBRX.go(leg4xPos,dur1);
        
        leg1z = interpFLZ.go(leg1zPos,dur1);
        leg2z = interpFRZ.go(leg2zPos,dur1);
        leg3z = interpBLZ.go(leg3zPos,dur1);
        leg4z = interpBRZ.go(leg4zPos,dur1);
        
        // send kimematic positions to legs

        // leg1, front right
        Serial1.print(1000);     // check value at start of data
        Serial1.print(','); 
        Serial1.print(leg1z);     // Z axis
        Serial1.print(','); 
        Serial1.print(leg1x);     // X axis
        Serial1.print('\n'); 

        // leg2, front left
        Serial2.print(1000);     // check value at start of data
        Serial2.print(','); 
        Serial2.print(leg2z);     // Z axis
        Serial2.print(','); 
        Serial2.print(leg2x);     // X axis
        Serial2.print('\n'); 

        // leg3, back right
        Serial3.print(1000);     // check value at start of data
        Serial3.print(','); 
        Serial3.print(leg3z);     // Z axis
        Serial3.print(','); 
        Serial3.print(leg3x);     // X axis
        Serial3.print('\n'); 

        // leg4, back right
        Serial4.print(1000);     // check value at start of data
        Serial4.print(','); 
        Serial4.print(leg4z);     // Z axis
        Serial4.print(','); 
        Serial4.print(leg4x);     // X axis
        Serial4.print('\n');

        //debug

        /*
        Serial.print(leg1z);
        Serial.print(" , ");
        Serial.print(leg1x);
        Serial.print(" , ");
        Serial.print(leg2z);
        Serial.print(" , ");
        Serial.print(leg2x);
        Serial.print(" , ");
        Serial.print(leg3z);
        Serial.print(" , ");
        Serial.print(leg3x);
        Serial.print(" , ");
        Serial.print(leg4z);
        Serial.print(" , ");
        Serial.print(leg4x);
        Serial.println();
        */

 
  } // end of timed loop

}  // end of main loop

// **** ISRs for reading encoders ***

// encoder 1

void doEncoderA(){  

  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {  
      encoder0Pos = encoder0Pos + 1;         // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinB) == HIGH) {   
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  } 
}

void doEncoderB(){  

  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {  
      encoder0Pos = encoder0Pos + 1;         // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinA) == LOW) {   
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }  

}
