//// Check functionality of two buttons

// Press button A to switch on LED, then press button B to switch it off
// Button A inturrupts should be ignored unless LED is on and vice versa for button B

#include <Arduino.h>

//Define constants ------------------------------------
#define BTNA 27  // check this matches hardware
#define BTNB 26
#define LED 13

//Setup variables ------------------------------------
volatile bool buttonAIsPressed = false;
volatile bool buttonBIsPressed = false;
int state = 0;

//Initialization ------------------------------------
void IRAM_ATTR pressedA() {  // the function to be called when interrupt is triggered
  buttonAIsPressed = true; 
}

void IRAM_ATTR pressedB() {  // the function to be called when interrupt is triggered
  buttonBIsPressed = true; 
}

void setup() {
  // set button and LED pins
  pinMode(BTNA, INPUT);
  pinMode(BTNB, INPUT);
  pinMode(LED, OUTPUT);

  // attatch interrupts to buttons
  attachInterrupt(BTNA, pressedA, RISING); 
  attachInterrupt(BTNB, pressedB, RISING); 
  
  // start serial for debugging
  Serial.begin(115200);

  // ensure LED is off before starting 
  digitalWrite(LED, LOW);
}

//Main loop ------------------------------------
void loop() {
  switch (state){
    
    // waiting for A to be pressed
    case 0:
      if (buttonAIsPressed) {
        digitalWrite(LED, HIGH);  // turn on LED indicator
        buttonBIsPressed = false; // ensure button B flag is off before starting to look for the press
        state = 1;        
      }

    // waiting for B to be pressed
    case 1:
      if (buttonBIsPressed) {
        digitalWrite(LED, LOW);  // turn on LED indicator
        buttonAIsPressed = false; // ensure button A flag is off before starting to look for the press
        state = 0;     
      }
  }
}
