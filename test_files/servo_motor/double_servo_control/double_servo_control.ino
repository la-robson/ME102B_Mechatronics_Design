// Double Servo with button and pot control

// Two buttons, one for each servo, when button is pressed the 

#include <Arduino.h>
#include <ESP32Servo.h>


// Setup --------------------------------

// define pins
#define SEV1 26
#define SEV2 21
#define POT 36
#define BTN1 4
#define BTN2 39
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
const int low_speed = 9;

// define button variables and isrs
volatile bool button1IsPressed = false;
volatile bool button2IsPressed = false;

void IRAM_ATTR pressed1_isr() {  // the function to be called when btn pressed
  button1IsPressed = true; 
}

void IRAM_ATTR pressed2_isr() {  // the function to be called when btn pressed
  button2IsPressed = true; 
}

// create servo objects to control servos
Servo myservo1;  
Servo myservo2;

void setup() {
  // Setup pins
  pinMode(LED, OUTPUT);
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(POT, INPUT);
  myservo1.attach(SEV1);
  myservo2.attach(SEV2);

  // attatch interrupts to buttons
  attachInterrupt(BTN1, pressed1_isr, RISING);
  attachInterrupt(BTN2, pressed2_isr, RISING);

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
      // if buttons pressed
      if (button1IsPressed) {state = 1;}
      if (button2IsPressed) {state = 2;}
      break;

    // servo1 moving 
    case 1:
      digitalWrite(LED, HIGH);   // turn on LED 
      // get curr speed from pot
      pot_reading  = analogRead(POT);
      curr_speed = pot_map(pot_reading);
      // scan from 0 to max and back at the selected speed
      servo1_move(0, max_pos, curr_speed);
      delay(200);
      servo1_move(max_pos, 0, curr_speed);
      digitalWrite(LED, LOW);   // turn off LED 
      // return to idle
      button1IsPressed = false;
      state = 0;    
      break;


    // servo 2 moving 
    case 2:
      digitalWrite(LED, HIGH);   // turn on LED 
      // get curr speed from pot
      pot_reading  = analogRead(POT);
      curr_speed = pot_map(pot_reading);    
      // scan from 0 to max and back at the selected speed
      servo2_move(0, max_pos, curr_speed);
      delay(200);
      servo2_move(max_pos, 0, curr_speed);
      digitalWrite(LED, LOW);   // turn off LED 
      // return to idle
      button2IsPressed = false;
      state = 0;    
      break;
  }
  
}


// Functions ------------------------

// function to move motor from start_pos to end_pos at speed (speed is time delay between each position step
void servo1_move(int start_pos, int end_pos, int servo_speed){
  // if start and end position are equal do nothing
  if (start_pos == end_pos){} 
  // if start is less than end step up to end_pos
  else if (start_pos < end_pos) { 
      for (pos = start_pos; pos <= end_pos; pos += 1) {
        myservo1.write(pos);
        delay(servo_speed);
      }
  // if start is more than end step down to end_pos    
  } else { 
      for (pos = start_pos; pos >= end_pos; pos -= 1) {
        myservo1.write(pos);
        delay(servo_speed);
      }
  }
}

// function to move motor from start_pos to end_pos at speed (speed is time delay between each position step
void servo2_move(int start_pos, int end_pos, int servo_speed){
  // if start and end position are equal do nothing
  if (start_pos == end_pos){} 
  // if start is less than end step up to end_pos
  else if (start_pos < end_pos) { 
      for (pos = start_pos; pos <= end_pos; pos += 1) {
        myservo2.write(pos);
        delay(servo_speed);
      }
  // if start is more than end step down to end_pos    
  } else { 
      for (pos = start_pos; pos >= end_pos; pos -= 1) {
        myservo2.write(pos);
        delay(servo_speed);
      }
  }
}
