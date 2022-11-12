// Main script for team 23 ME102B project

// import modules ------------------------------
#include <Arduino.h>
#include <ESP32Servo.h>

// define variables ------------------------------

// pins
#define trapdoor_servo_pin 13  // change these to match wiring
#define feeder_servo_pin 27
#define feed_button_pin 42 
#define throw_button_pin 34
#define pot_pin 12

// basics
int state = 1;

// ball launcher
volatile bool ball_throw_state = false;   // true indicates throwing in progress (block other events)
volatile bool ball_throw_timer = false;
volatile bool ball_throw_switch = false;


// feeder
volatile bool feeder_state = false;   // true indicates feeding in progress (block other events)
volatile bool feeder_timer = false;
int feed_count = 0;
const int max_feed_count = 10; 


// objects
Servo trapdoor_servo;  // create servo object to control a servo
Servo feeder_servo;  // create servo object to control a servo

// timers -----------------------------------------------

// define timers and their flags

// playtime - automatic start of throw game 
volatile bool playtime = false;
hw_timer_t * play_timer = NULL;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;

// mealtime - automatic dispensing of food
volatile bool mealtime = false;
hw_timer_t * meal_timer = NULL;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;

// throw_timeout - time period to throw ball
volatile bool throw_timeout_flag = false;
hw_timer_t * throw_timeout = NULL;
portMUX_TYPE timerMux2 = portMUX_INITIALIZER_UNLOCKED;

// feed timeout - time period to dispense food 
volatile bool feed_timeout_flag = false;
hw_timer_t * feed_timeout = NULL;
portMUX_TYPE timerMux3 = portMUX_INITIALIZER_UNLOCKED;


// timer ISRs

// playtime isr
void IRAM_ATTR onPlaytime() {
  portENTER_CRITICAL_ISR(&timerMux0);
  playtime = true;
  portEXIT_CRITICAL_ISR(&timerMux0);
}

// mealtime isr
void IRAM_ATTR onMealtime() {
  portENTER_CRITICAL_ISR(&timerMux1);
  mealtime = true;
  portEXIT_CRITICAL_ISR(&timerMux1);
}

// throw_timeout isr
void IRAM_ATTR onThrowTimeout() {
  portENTER_CRITICAL_ISR(&timerMux2);
  throw_timeout_flag = true;
  portEXIT_CRITICAL_ISR(&timerMux2);
}

// feed_timeout isr
void IRAM_ATTR onFeedTimeout() {
  portENTER_CRITICAL_ISR(&timerMux3);
  feed_timeout_flag = true;
  portEXIT_CRITICAL_ISR(&timerMux3);
}

// setup ------------------------------
void setup() {
  Serial.begin(115200); // DELETE FROM FINAL CODE -- DEBUG TOOL ONLY

  // connect servo pins 
  feeder_servo.attach(feeder_servo_pin);
  trapdoor_servo.attach(trapdoor_servo_pin);

  // connect button and pot pins 
  pinMode(feed_button_pin, INPUT);
  pinMode(throw_button_pin, INPUT);
  pinMode(pot_pin, INPUT);

  // set up encoder for DC motors

  // initialise timers
  // begin , attach interupt, alarm write, alarm enable

  
}

// main loop ------------------------------
void loop() {

  switch (state){
    ///// Ball launcher states

    // thrower idle
    case 1:
      if (ball_throw_start_checker()){
        ball_throw_start();
        state = 2;
      } else if (feed_start_checker()){
        // start feed timer 
        // open feeder
        state = 4;
      }
  
    // throwing ball
    case 2:
      if (ball_throw_stop_checker){
        ball_throw_stop();
        state = 3;
      }
    
    // waiting for ball return
    case 3:
      if (throw_feed_count_checker()){
        // start  dispensing food
        state = 4;
      } if (!throw_feed_count_checker()){
        thrower_idle();
        state = 1;
      }
    
  
    ///// Feeder states
  
    // dispensing food
    case 4:
      if (food_stop_checker){
        dispense_food_stop();
        state = 1;
      }

  }
}
