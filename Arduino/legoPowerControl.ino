/*
Control lego power motor and servo. They are connected via mtor shield.
Receive a command from serial port (usb). The commande is a single character:
receive: action:
+        increase motor speed
-        decrease motor speed
0        stop motor
F        motor forward
B        motor backward
<        turn more left
>        turn more right
|        center direction
*/

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  
  // needed by the amazing Christmas tree feature
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  char c;
  if(Serial.available()) {  
    c = Serial.read();  
    Serial.print(c); 
    //@todo: change to do the specified action
    if (c == '+') {
      blinkLed(1);
    }
    if (c == '-') {
      blinkLed(2);
    }
  }
  
    
  // Enlights a beautiful 3 leds Christmas tree ! What an amazing feature !
  int pinsArray[3] = {13,12,11};
  chrismasTree(pinsArray);
}

void blinkLed(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(150);                
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}

void christmasTree(int pins[3]) {
  short int i;
  for(i=3;i>=0;i--) {
    digitalWrite(pins[i], HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);
    digitalWrite(pins[i], LOW);    // turn the LED off by making the voltage LOW
  }
}

