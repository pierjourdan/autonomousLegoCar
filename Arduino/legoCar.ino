/*
Code adapted from http://playground.arduino.cc/Main/AdafruitMotorShield
Open Source / Public Domain
*/
// Arduino pins for the shift register
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

// 8-bit bus after the 74HC595 shift register
// (not Arduino pins)
// These are used to set the direction of the bridge driver.
int motorA[] = {2, 1, 5, 0};
int motorB[] = {3, 4, 7, 6};

// Arduino pins for the PWM signals. The first array eleemnt is for motor 1 output, etc.. 
int motorPwms[] = {11, 3, 6, 5};

//value set to the motor: beetween 0 and 255
int motorValues[] = {0, 0, 0, 0};

void setup()
{
  Serial.begin(9600);
  Serial.println("Ca demarre");
  
  delay(1000);
  motorAdd(0, 200);//go forward, speed 120 (max is 255)
  delay(1000);
  
  motorAdd(1, -180);//turn left 180 (max is 255)
  delay(3500);

  motorAdd(1, 180);//turn right 180 (max is 255)
  delay(3000);

  release(0);//stop motor 0 (connected to M1 on motor shield)
}

void loop()
{

}
/*
void loop()
{

    char c;
  
  if (Serial.available()) {  
    c = Serial.read();  
    Serial.print(c); 

    switch (c) {
      case '+':
        motorAdd(0, 30);
        Serial.println(" En avant !!! ");
        break;
      case '-':
        motorAdd(0, -30);
        Serial.println(" arriere ");
        break;
      case '0':
        release(0);
        Serial.println(" /!\\ stop ");
        break;
        
      case '6':
        motorAdd(1, 30);
        Serial.println(" A droite ");
        break;
      case '4':
        motorAdd(1, -30);
        Serial.println(" A gauche ");
        break;
      case '5':
        release(1);
        Serial.println(" Tout droit ");
        break;
    }

    Serial.println(motorSpeed);
    Serial.println(directionServo);
  }

}
*/

void motorAdd(int motorNumber, int speedToAdd)
{
  motorValues[motorNumber] +=  speedToAdd;
  motorValues[motorNumber] = constrain(motorValues[motorNumber], -255, 255);

  if (motorValues[motorNumber] == 0) {
    release(motorNumber);
    return;
  } 

  if (motorValues[motorNumber] > 0) {
    forward(motorNumber, motorValues[motorNumber]);
  } else {
    backward(motorNumber, abs(motorValues[motorNumber]));
  }

}

void forward(int motorNumber, int speed)
{
  speed = constrain(speed, 0, 255);
  motor_output(motorPwms[motorNumber], motorA[motorNumber], HIGH, speed);
  motor_output(motorPwms[motorNumber], motorB[motorNumber], LOW, -1);     // -1: no PWM set
}

void backward(int motorNumber, int speed)
{
  speed = constrain(speed, 0, 255);
  motor_output (motorPwms[motorNumber], motorA[motorNumber], LOW, speed);
  motor_output (motorPwms[motorNumber], motorB[motorNumber], HIGH, -1);    // -1: no PWM set
}

void release(int motorNumber)
{
  motorValues[motorNumber] = 0;
  motor_output(motorPwms[motorNumber], motorA[motorNumber], LOW, 0);  // 0: output floating.
  motor_output(motorPwms[motorNumber], motorB[motorNumber], LOW, -1); // -1: no PWM set
}

// ---------------------------------
// motor_output
//
// The function motor_ouput uses the motor driver to
// drive normal outputs like lights, relays, solenoids,
// DC motors (but not in reverse).
//
// It is also used as an internal helper function
// for the motor() function.
//
// The high_low variable should be set 'HIGH'
// to drive lights, etc.
// It can be set 'LOW', to switch it off,
// but also a 'speed' of 0 will switch it off.
//
// The 'speed' sets the PWM for 0...255, and is for
// both pins of the motor output.
//   For example, if motor 3 side 'A' is used to for a
//   dimmed light at 50% (speed is 128), also the
//   motor 3 side 'B' output will be dimmed for 50%.
// Set to 0 for completelty off (high impedance).
// Set to 255 for fully on.
// Special settings for the PWM speed:
//    Set to -1 for not setting the PWM at all.
//
void motor_output (int motorPwm, int output, int high_low, int speed)
{
  // Set the direction with the shift register
  shiftWrite(output, high_low);
  if (speed != -1) {
    speed = constrain(speed, 0, 255);
    analogWrite(motorPwm, speed);
  }
}


// ---------------------------------
// shiftWrite
//
// The parameters are just like digitalWrite().
//
// The output is the pin 0...7 (the pin behind
// the shift register).
// The second parameter is HIGH or LOW.
//
// There is no initialization function.
// Initialization is automatically done at the first
// time it is used.
//
void shiftWrite(int output, int high_low)
{
  static int latch_copy;
  static int shift_register_initialized = false;

  // Do the initialization on the fly,
  // at the first time it is used.
  if (!shift_register_initialized) {
    // Set pins for shift register to output
    pinMode(MOTORLATCH, OUTPUT);
    pinMode(MOTORENABLE, OUTPUT);
    pinMode(MOTORDATA, OUTPUT);
    pinMode(MOTORCLK, OUTPUT);

    // Set pins for shift register to default value (low);
    digitalWrite(MOTORDATA, LOW);
    digitalWrite(MOTORLATCH, LOW);
    digitalWrite(MOTORCLK, LOW);
    // Enable the shift register, set Enable pin Low.
    digitalWrite(MOTORENABLE, LOW);

    // start with all outputs (of the shift register) low
    latch_copy = 0;

    shift_register_initialized = true;
  }

  // The defines HIGH and LOW are 1 and 0.
  // So this is valid.
  bitWrite(latch_copy, output, high_low);

  // Use the default Arduino 'shiftOut()' function to
  // shift the bits with the MOTORCLK as clock pulse.
  // The 74HC595 shiftregister wants the MSB first.
  // After that, generate a latch pulse with MOTORLATCH.
  shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(MOTORLATCH, HIGH);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(MOTORLATCH, LOW);
}


