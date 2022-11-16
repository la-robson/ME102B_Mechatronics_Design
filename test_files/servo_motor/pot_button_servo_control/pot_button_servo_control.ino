// Servo with button and pot control

#include <Arduino.h>
#include <ESP32Servo.h>


// Setup --------------------------------

// define pins
#define SEV 26
#define POT 36
#define BTN 4
#define LED 13

// define basic variables 
int state = 0;

// define potentiometer variables
int pot_reading;                  // current potentiometer reading
const int max_pot_reading = 4095; // define maximum potentiometer reading
int output_val;                   // current output value

// define servo variables
int pos = 0;    // variable to store the servo position
const int max_pos = 90; // max angle
int curr_speed;
const int high_speed = 1;
const int med_speed = 5;
const int low_speed = 15;

// define button variables and isr
volatile bool buttonIsPressed = false;

void IRAM_ATTR pressed_isr() {  // the function to be called when btn pressed
  buttonIsPressed = true; 
}

Servo myservo;  // create servo object to control a servo

void setup() {
  // Setup pins
  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT);
  pinMode(POT, INPUT);
  myservo.attach(SEV);

  // attatch interrupts to buttons
  attachInterrupt(BTN, pressed_isr, RISING);

  // start serial for debugging
  Serial.begin(115200);

  // ensure LED is off before starting 
  digitalWrite(LED, LOW);
}



//Main loop ------------------------------------
void loop() {
  switch(state){
    
    // idle
    case 0:
      // if button pressed
      if (buttonIsPressed) {
        state = 1; 
      }
      break;

    // servo moving 
    case 1:
      digitalWrite(LED, HIGH);   // turn on LED 
  
      // get curr speed from pot
      pot_reading  = analogRead(POT);
      curr_speed = pot_map(pot_reading);
      
      
      // scan from 0 to max and back at the selected speed
      servo_move(0, max_pos, curr_speed);
      delay(2000);
      servo_move(max_pos, 0, curr_speed);

      digitalWrite(LED, LOW);   // turn off LED 
      
      // return to idle
      buttonIsPressed = false;
      state = 0;    
      break;
  }
  
}


// Functions ------------------------

// function to move motor from start_pos to end_pos at speed (speed is time delay between each position step
void servo_move(int start_pos, int end_pos, int servo_speed){
  // if start and end position are equal do nothing
  if (start_pos == end_pos){} 
  // if start is less than end step up to end_pos
  else if (start_pos < end_pos) { 
      for (pos = start_pos; pos <= end_pos; pos += 1) {
        myservo.write(pos);
        delay(servo_speed);
      }
  // if start is more than end step down to end_pos    
  } else { 
      for (pos = start_pos; pos >= end_pos; pos -= 1) {
        myservo.write(pos);
        delay(servo_speed);
      }
  }
}

// funtion to map the potentiometer value to low med high _speed
int pot_map(int reading){
  if (reading < max_pot_reading/3) {return low_speed;}
  else if (reading < max_pot_reading*2/3) {return med_speed;}
  else {return high_speed;}
}
