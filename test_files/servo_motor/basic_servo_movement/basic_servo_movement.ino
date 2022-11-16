// Basic servo motion
// function to move from start to end position at a given speed


#include <Arduino.h>
#include <ESP32Servo.h>

#define SVPIN 26

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
const int max_pos = 200; // max angle
const int high_speed = 1;
const int med_speed = 5;
const int low_speed = 15;


void setup() {
  myservo.attach(SVPIN);  // attaches the servo on pin 13 to the servo object
}

void loop() {
  servo_move(0, max_pos, low_speed);
  servo_move(max_pos, 0, high_speed);
  delay(2000);
}


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
