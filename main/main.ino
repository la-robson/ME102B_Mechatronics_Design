/* ME102b  project 
 *  Pet entertainment device control
 *  Script to control a tennis ball launcher and treat dispenser
 *  Authors: Lauren Robson, Travis Carol
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



//Setup variables ------------------------------------
// basic variables
int state = 1;

// define throw servo variables
int pos = 0;    // variable to store the servo position
const int max_pos = 90; // max angle
const int high_speed = 1;
const int med_speed = 5;
const int low_speed = 15;
int curr_speed = low_speed;
Servo th_servo;  // create servo object to control servos
Servo fd_servo;

// Pwm variables for motor
const int freq = 5000;
const int ledChan_10 = 10;
const int ledChan_11 = 11;
const int res = 8;
const int MAXPWM = 255;

// DC Motor variables
int mtrS = 0;

// potentiometer variables
int potRead = 0;

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

// playtime - automatic start of throw game 
volatile bool playtime = false;
int playtime_period = 5000000*1 ;  // period * 1 us (5 mil = 5s)
hw_timer_t * play_timer = NULL;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;

// playtime isr
void IRAM_ATTR onPlaytime() {
  portENTER_CRITICAL_ISR(&timerMux0);
  playtime = true;
  portEXIT_CRITICAL_ISR(&timerMux0);
}

// mealtime - automatic dispensing of food
volatile bool mealtime = false;
int mealtime_period = 20000000;  // period * 1 us
hw_timer_t * meal_timer = NULL;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;

// mealtime isr
void IRAM_ATTR onMealtime() {
  portENTER_CRITICAL_ISR(&timerMux1);
  mealtime = true;
  portEXIT_CRITICAL_ISR(&timerMux1);
}


//Initialization ------------------------------------
void setup() {
  // set button and LED pins
  pinMode(TH_BTN, INPUT);
  pinMode(FD_BTN, INPUT);
  pinMode(SWITCH, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(POT,INPUT);
  th_servo.attach(TH_SEV);
  fd_servo.attach(FD_SEV);

  // attatch interrupts
  attachInterrupt(SWITCH, switch_isr, RISING); 
  attachInterrupt(TH_BTN, throw_button_isr, RISING); 
  attachInterrupt(FD_BTN, feed_button_isr, RISING); 

  // set channels for DC motor
  ledcSetup(ledChan_10, freq, res); // channel 0 setup
  ledcSetup(ledChan_11, freq, res); // channel 1 setup
  ledcAttachPin(mtrb1,ledChan_10);
  ledcAttachPin(mtrb2,ledChan_11);

  // initialise timers 
  timer_init();
  
  Serial.begin(115200);
}

// main loop ---------------------------------------
void loop() {

  switch (state){
    
    // idle
    case 1:
      Serial.println("In state 1");
      if (throwButtonPressed or playtime) {state = 2;}
      if (feedButtonPressed) {state = 4;}
      break;


    // launching ball
    case 2:
      Serial.println("In state 2");
      potRead = analogRead(POT);  // read pot
      digitalWrite(LED, HIGH); // indicator LED on
      th_servo_move(0, max_pos, curr_speed); // open trapdoor
      dc_motor_routine(); // run motor
      th_servo_move(max_pos, 0, curr_speed); // close trapdoor
      digitalWrite(LED, LOW); // indicator LED off
      
      // go to wait for ball return
      switchPressed = false; 
      state = 3;
      break;


    // waiting for ball return
    case 3:
      Serial.println("In state 3");
      // flash LED
      flash_LED();
      // check for ball return
      if (switchPressed){
        throwButtonPressed = false;
        playtime = false;
        timerRestart(play_timer); // restart the playtime timer
        state = 1;
        }
      break;


    // dispensing food
    case 4:
      Serial.println("In state 4");
      // dispense food
      fd_servo_move(0, max_pos, curr_speed); // open feeder gate 
      wait(2000); // delay 
      fd_servo_move(max_pos, 0, curr_speed); // close feeder gate
      
      // return to idle state 
      // reset flags
      feedButtonPressed = false;
      state = 1;
      break;

  }
}


// define functions -------------------

// timer init functions ---
void timer_init(){
  // playtime
  play_timer = timerBegin(0, 80, true);  // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(play_timer, onPlaytime, true); // edge (not level) triggered
  timerAlarmWrite(play_timer, playtime_period, true); // period * 1 us, autoreload true
  timerAlarmEnable(play_timer); // enable
  timerRestart(play_timer);
  // mealtime
  meal_timer = timerBegin(0, 80, true);  // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(play_timer, onMealtime, true); // edge (not level) triggered
  timerAlarmWrite(meal_timer, mealtime_period, true); // period * 1 us, autoreload true
  timerAlarmEnable(meal_timer); // enable
  timerRestart(meal_timer);
}

// DC motor functions ---

// start motor
void motor_start(){
  ledcWrite(ledChan_10,mtrS);
  ledcWrite(ledChan_11,LOW);
}

// stop motor
void motor_stop(){
  ledcWrite(ledChan_10,LOW);
  ledcWrite(ledChan_11,LOW);
}

// DC motor routine
void dc_motor_routine(){
      mtrS = MAXPWM;  // set speed
      motor_start(); // start 
      wait(2000);     // run for 2s
      motor_stop();   // stop
}
