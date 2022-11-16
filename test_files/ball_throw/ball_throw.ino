// Logic for the ball throw side of the system

#include <Arduino.h>
#include <ESP32Servo.h>

//Define constants ------------------------------------
#define POT 14
#define SWITCH 21  // limit switch for ball return sensing 
#define TH_BTN 34  // throw button
#define mtrb1 25   // dc motor control
#define mtrb2 26   // dc motor control
#define TH_SEV 23   // trapdoor servo
#define LED 13     // indicator LED



//Setup variables ------------------------------------
// basic variables
int state = 0;


// define throw servo variables
int pos = 0;    // variable to store the servo position
const int max_pos = 90; // max angle
const int high_speed = 1;
const int med_speed = 5;
const int low_speed = 15;
int curr_speed = low_speed;
Servo myservo;  // create servo object to control a servo

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

// playtime - automatic start of throw game 
volatile bool playtime = false;
int playtime_period = 5000000;  // period * 1 us
hw_timer_t * play_timer = NULL;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;

// playtime isr
void IRAM_ATTR onPlaytime() {
  portENTER_CRITICAL_ISR(&timerMux0);
  playtime = true;
  portEXIT_CRITICAL_ISR(&timerMux0);
}



//Initialization ------------------------------------
void setup() {
  // set button and LED pins
  pinMode(TH_BTN, INPUT);
  pinMode(SWITCH, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(POT,INPUT);
  myservo.attach(TH_SEV);

  // attatch interrupts
  attachInterrupt(SWITCH, switch_isr, RISING); 
  attachInterrupt(TH_BTN, throw_button_isr, RISING); 

  // set channels for DC motor
  ledcSetup(ledChan_10, freq, res); // channel 0 setup
  ledcSetup(ledChan_11, freq, res); // channel 1 setup
  ledcAttachPin(mtrb1,ledChan_10);
  ledcAttachPin(mtrb2,ledChan_11);

  // initialise timers 
  playtime_init();
  
  Serial.begin(115200);
}

// main loop ---------------------------------------
void loop() {

  switch (state){
    
    // idle
    case 0:
      if (throwButtonPressed or playtime) {state = 1;}
      Serial.println("in state 0");
      break;


    // launching ball
    case 1:
      Serial.println("in state 1");
           
      potRead = analogRead(POT);  // read pot
      digitalWrite(LED, HIGH); // indicator LED on
      Serial.println("LED on");
      servo_move(0, max_pos, curr_speed); // open trapdoor
      Serial.println("Open door");
      dc_motor_routine(); // run motor
      Serial.println("motor stop");
      servo_move(max_pos, 0, curr_speed); // close trapdoor
      Serial.println("close door");
      digitalWrite(LED, LOW); // indicator LED off
      Serial.println("led off");
      
      // go to wait for ball return
      switchPressed = false; 
      state = 2;
      break;


    // waiting for ball return
    case 2:
      Serial.println("in state 2");
      // flash LED
      digitalWrite(LED, HIGH);
      wait(200);
      digitalWrite(LED, LOW);
      wait(200);
      // check for ball return
      if (switchPressed){
        throwButtonPressed = false;
        playtime = false;
        timerRestart(play_timer); // restart the playtime timer
        state = 0;
        }
      break;

  }
}


// define functions -------------------

// timer init functions ---
void playtime_init(){
  play_timer = timerBegin(0, 80, true);  // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(play_timer, onPlaytime, true); // edge (not level) triggered
  timerAlarmWrite(play_timer, playtime_period, true); // period * 1 us, autoreload true
  timerAlarmEnable(play_timer); // enable
  timerRestart(play_timer);
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


// Servo Motor functions ---

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
