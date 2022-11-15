//// Check functionality of single button 

// Press button to return 1 in serial port and turn on LED for 1s

#include <Arduino.h>

//Define constants ------------------------------------
#define BTNA 39  // check this matches hardware
#define LED 13

//Setup variables ------------------------------------
volatile bool buttonAIsPressed = false;

//Initialization ------------------------------------
void IRAM_ATTR pressedA() {  // the function to be called when interrupt is triggered
  buttonAIsPressed = true; 
}


void setup() {
  // set button and LED pins
  pinMode(BTNA, INPUT);
  pinMode(LED, OUTPUT);

  // attatch interrupts to buttons
  attachInterrupt(BTNA, pressedA, RISING);
   
  // start serial for debugging
  Serial.begin(115200);

  // ensure LED is off before starting 
  digitalWrite(LED, LOW);
}

//Main loop ------------------------------------
void loop() {
  if (buttonAIsPressed){
    digitalWrite(LED, HIGH);
  }
  Serial.println("Button State: " + String(buttonAIsPressed));
  buttonAIsPressed = false;
  
  delay(1000);
  digitalWrite(LED, LOW);
}
