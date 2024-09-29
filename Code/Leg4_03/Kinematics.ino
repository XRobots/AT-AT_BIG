void kinematics (float xIn, float zIn) {

    #define length1 660 
    float angle1;
    float angle1a;
    float angle1b;
    float angle1c;
    float angle2;
    float angle2Degrees;
    float angle2aDegrees;
    float angle1Degrees;
    float z3;
    float shoulderAngle2;
    float shoulderAngle2Degrees;

    float shoulderAngleServo;
    float shoulderAngleServo2;
    float kneeAngleServo;


    // calculate the shoulder joint offset and new leg length based on now far the foot moves forward/backwards
    shoulderAngle2 = atan(xIn/zIn);     // calc how much extra to add to the shoulder joint    
    z3 = zIn/cos(shoulderAngle2);     // calc new leg length to feed to the next bit of code below

    // calculate leg length based on shin/thigh length and knee and shoulder angle
    angle1a = sq(length1) + sq(z3) - sq(length1);
    angle1b = 2 * length1 * z3;
    angle1c = angle1a / angle1b;
    angle1 = acos(angle1c);     // hip angle RADIANS
    angle2 = PI - (angle1 * 2); // knee angle RADIANS    

    //calc degrees from angles
    angle2Degrees = angle2 * (180/PI);    // knee angle DEGREES
    angle2aDegrees = angle2Degrees / 2;    // half knee angle for each servo DEGREES
    shoulderAngle2Degrees = shoulderAngle2 * (180/PI);    // front/back shoulder offset DEGREES

    kneeAngleServo = (90-angle2aDegrees)*15.5;
    shoulderAngleServo = (shoulderAngle2Degrees)*15.5;

/*
    Serial.print(zIn);
    Serial.print(" , ");
    Serial.print(xIn);
    Serial.print(" , ");
    Serial.print(kneeAngleServo);
    Serial.print(" , ");
    Serial.print(shoulderAngleServo);
    Serial.print(" , ");
    Serial.print(Setpoint1);
    Serial.print(" , ");
    Serial.println(Setpoint2);

*/

    // servo stuff  
    // centre points of pots
    
    Setpoint1 = (kneeAngleServo)-shoulderAngleServo;
    Setpoint2 = (kneeAngleServo*-1)-shoulderAngleServo;
    
    // constrain endstops
    Setpoint1 = constrain(Setpoint1, -435, 435);
    Setpoint2 = constrain(Setpoint2, -435, 435);
    
    Input1 = potTop;
    Input2 = potBottom;
    PID1.Compute();
    PID2.Compute();

  
    if (switch1 == 0) {           // motor enable
        // Top of leg
        if (Output1 > 0) {
          analogWrite(19, Output1);
          analogWrite(18, 0);
        }
        else if (Output1 < 0) {
          Output1a = abs(Output1);
          analogWrite(18, Output1a);
          analogWrite(19, 0);
        }
        else {
          analogWrite(19, 0);
          analogWrite(18, 0);
        }
      
        // Bottom of leg
        if (Output2 > 0) {
          analogWrite(23, Output2);
          analogWrite(22, 0);
        }
        else if (Output2 < 0) {
          Output2a = abs(Output2);
          analogWrite(22, Output2a);
          analogWrite(23, 0);
        }
        else {
          analogWrite(23, 0);
          analogWrite(22, 0);
        }
    }
    else if (switch1 == 1) {      // motor disable
      analogWrite(23, 0);
      analogWrite(22, 0);
      analogWrite(19, 0);
      analogWrite(18, 0);
    }
}
