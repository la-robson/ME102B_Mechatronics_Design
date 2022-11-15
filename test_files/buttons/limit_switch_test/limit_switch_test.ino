//// Test limit switch

// when limit switch is pressed LED will do a flashing routine then return to looking for limit switch inturrupt

#include <Arduino.h>

//Define constants ------------------------------------
#define SWTCH 27  // check this matches hardware
#define LED 13

//Setup variables ------------------------------------
volatile bool switchPressed = false;
int state = 0;

//Initialization ------------------------------------
void IRAM_ATTR switch_isr() {  // the function to be called when interrupt is triggered
  switchPressed = true; 
}

void setup() {
  // set button and LED pins
  pinMode(SWTCH, INPUT);
  pinMode(LED, OUTPUT);

  // attatch interrupt to switch
  attachInterrupt(SWTCH, switchPressed, RISING); 
  
  // start serial for debugging
  Serial.begin(115200);

  // ensure LED is off before starting 
  digitalWrite(LED, LOW);
}

//Main loop ------------------------------------
void loop() {
  switch (state){
    
    // waiting for switch to be pressed
    case 0:
      if (switchPressed) {
        state = 1;        
      }

    // led flashing routine
    case 1:
      for (int i=0; i<5; i++){
        digitalWrite(LED, HIGH);
        delay(200)
        digitalWrite(LED, LOW);
        delay(200)
      }
      switchPressed = false; // ensure button A flag is off before starting to look for the press
      state = 0;     
  }
}
