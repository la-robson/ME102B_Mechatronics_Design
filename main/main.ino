/* ME102b  project 
 *  Pet entertainment device control
 *  Script to control a tennis ball launcher and treat dispenser
 *  Authors: Lauren Robson, Travis Carroll
 */

#include <Arduino.h>
#include <ESP32Servo.h>

//Define constants ------------------------------------
#define POT 14
#define SWITCH 21  // limit switch for ball return sensing 
#define TH_BTN 39  // throw button
#define FD_BTN 36    // feeder button
#define mtrb1 25   // dc motor control
#define mtrb2 26   // dc motor control
#define TH_SEV 23   // trapdoor servo
#define FD_SEV 4    // feeder servo
#define LED 13     // indicator LED
#define En_R 12   //enable pwm pin 
#define En_L 12


//Setup variables ------------------------------------


// basic operation variables
int state = 1;
int feed_count = 0; // number of treats dispensed 
const int max_feed_count = 3;   // max number of treats allowed between mealtimes


// potentiometer variables
int pot_reading;                  // current potentiometer reading
const int max_pot_reading = 4095; // define maximum potentiometer reading
int feed_time;
const int large_feed = 1000;
const int med_feed = 750;
const int small_feed = 400;

// define throw servo variables
int pos = 0;    // variable to store the servo position
const int max_pos = 90; // max angle
const int high_speed = 1;
const int med_speed = 5;
const int low_speed = 15;
int curr_speed = high_speed;
Servo th_servo;  // create servo objects
Servo fd_servo;

// DC Motor variables
const int freq = 5000;
const int ledChan_10 = 10;
const int ledChan_11 = 11;
const int res = 8;
const int MAXPWM = 150;
int mtrS = 0;

// limit switch variables and isr
volatile bool switchPressed = false;
void IRAM_ATTR switch_isr() {  // the function to be called when interrupt is triggered
  switchPressed = true;   
}

// throw button variables and isr
volatile bool throwButtonPressed = false;
void IRAM_ATTR throw_button_isr() {  // the function to be called when interrupt is triggered
  throwButtonPressed = true; 
}

// feed button variables and isr
volatile bool feedButtonPressed = false;
void IRAM_ATTR feed_button_isr() {  // the function to be called when interrupt is triggered
  feedButtonPressed = true; 
  
}




// mealtime - automatic dispensing of food
volatile int mealtime = 0;      // mealtime flag, 0-not triggered, 1-triggered but not "seen", 2-dispensing meal
int mealtime_period = 60*1000000;  // period * 1 us (set to 60s for testing)
hw_timer_t * meal_timer = NULL;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;

// mealtime isr
void IRAM_ATTR onMealtime() {
  portENTER_CRITICAL_ISR(&timerMux1);
  mealtime = 1;
  portEXIT_CRITICAL_ISR(&timerMux1);
}


//Initialization ------------------------------------
void setup() {
  pins_init();    // set button and LED pins
  interrupt_init();   // attatch interrupts
  dc_motor_init();   // set dc motor channels
  timer_init();   // initialise timers 
  Serial.begin(115200);   // start seria for debug
}

// main loop ---------------------------------------
void loop() {

switch (state){
    
    // idle
    case 1:
      Serial.println("In state 1");
      
      if (throwButtonPressed) {state = 2;}
      else if ((feedButtonPressed) and (feed_count <= max_feed_count)) {to_treat_state();}
      else if (mealtime == 1) {to_mealtime();}
      break;


    // launching ball
    case 2:
      Serial.println("In state 2");
      throw_ball(); // throw ball
      to_wait_4ball(); // go to wait for ball return
      break;


    // waiting for ball return
    case 3:
      Serial.println("In state 3");
      flash_LED();  // flash LED to indicate waiting for ball
      // check for ball return
      if (switchPressed){
        throw_ball_reset(); // reset flags and timer       
        
        // if allowed more treats get treat, if over max go back to idle
        if (feed_count <= max_feed_count) {to_treat_state();}
        else {to_idle_state();}
      }
      break;
      

    // dispensing food
    case 4:
      Serial.println("In state 4");
      dispense_food();  // dispense food
      to_idle_state();  // return to idle state 
      break;

  }
}
