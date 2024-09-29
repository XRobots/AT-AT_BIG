// ***** ACTUATOR ****

int button1;
int button2;
int switch1;

int data;

unsigned long currentMillis;
unsigned long previousMillis = 0;        // set up timers
long interval = 10;                      // time constant for timer

void setup() {
  
  Serial.begin(115200);   // debug
  Serial1.begin(115200);  // actuator data
  
  pinMode(13, OUTPUT);        // diags LED
  
  pinMode(4, INPUT_PULLUP);   //  button1
  pinMode(3, INPUT_PULLUP);   //  button2
  pinMode(2, INPUT_PULLUP);   //  switch1
  
  pinMode(14, OUTPUT);        // upper leg PWM
  pinMode(15, OUTPUT);

}

void loop() {

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
  
  if (Serial1.available() > 1){           // receive IMU data from serial
        digitalWrite(13, HIGH);           // serial data is there
        data = Serial1.parseInt();
        if (Serial1.read() == '\n') {     // end of data             
            }
  }
  else digitalWrite(13, LOW);    // no serial data

  button1 = digitalRead(4);
  button2 = digitalRead(3);
  switch1 = digitalRead(2);

  Serial.print(data);
  Serial.print(" , ");
  Serial.print(button1);
  Serial.print(" , ");
  Serial.print(button2);
  Serial.print(" , ");
  Serial.print(switch1);
  Serial.println();

  

  // manual homing buttons

  if (switch1 == 1) {
      if (button1 == 0) { 
          analogWrite(14, 255);
          analogWrite(15, 0);
        }
      else if (button2 == 0) {
          analogWrite(14, 0);
          analogWrite(15, 255);
        }
      else {
          analogWrite(14, 0);
          analogWrite(15, 0);
      }
  }

  // drive actuator drom data

  if (data == 600) {          // open actuator
    analogWrite(14, 255);
    analogWrite(15, 0);
  }
  else if (data == 700) {     // close actuator
    analogWrite(14, 0);
    analogWrite(15, 255);
  }
  else if (data == 500) {     // stop actuator
    analogWrite(14, 0);
    analogWrite(15, 0);
  }

  
  }  // end of timed loop

} // end of main loop


