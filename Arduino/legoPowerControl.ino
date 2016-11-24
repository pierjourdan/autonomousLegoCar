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
}

/* Bonjour */

void blinkLed(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(150);                
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}

/* Mon but est de faire un conflict :) */

