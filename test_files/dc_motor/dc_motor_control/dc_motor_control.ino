#include <ESP32Encoder.h>
#define BIN_1 26
#define BIN_2 25

ESP32Encoder encoder;


int D = 0;

int Kp = 63;   // TUNE THESE VALUES TO CHANGE CONTROLLER PERFORMANCE
int Ki = 15;
int IMax = 105;

//Setup interrupt variables ----------------------------
volatile int count = 0; // encoder count
volatile bool deltaT = false;     // check timer interrupt 2
hw_timer_t * timer1 = NULL;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;

// setting PWM properties ----------------------------
const int freq = 5000;
const int ledChannel_1 = 1;
const int ledChannel_2 = 2;
const int resolution = 8;
const int MAX_PWM_VOLTAGE = 255;
const int NOM_PWM_VOLTAGE = 150;

//Initialization ------------------------------------


void IRAM_ATTR onTime1() {
  portENTER_CRITICAL_ISR(&timerMux1);
  count = encoder.getCount( );
  encoder.clearCount ( );
  deltaT = true; // the function to be called when timer interrupt is triggered
  portEXIT_CRITICAL_ISR(&timerMux1);
}


void setup() {

  ESP32Encoder::useInternalWeakPullResistors = UP; // Enable the weak pull up resistors
  encoder.attachHalfQuad(33, 27); // Attache pins for use as encoder pins
  encoder.setCount(0);  // set starting count value after attaching

  // configure LED PWM functionalitites
  ledcSetup(ledChannel_1, freq, resolution);
  ledcSetup(ledChannel_2, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(BIN_1, ledChannel_1);
  ledcAttachPin(BIN_2, ledChannel_2);

  // initilize timer
  timer1 = timerBegin(1, 80, true);  // timer 1, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer1, &onTime1, true); // edge (not level) triggered
  timerAlarmWrite(timer1, 10000, true); // 10000 * 1 us = 10 ms, autoreload true

  // at least enable the timer alarms
  timerAlarmEnable(timer1); // enable
}

// main loop ------------------------------------------------------
void loop() {
  
//  start_throw_motor();
  ledcWrite(ledChannel_1, 180);
  ledcWrite(ledChannel_2, LOW);
  delay(2000);
//  stop_throw_motor();
  ledcWrite(ledChannel_1, LOW);
  ledcWrite(ledChannel_2, LOW);
  delay(2000);
  
}

void start_throw_motor(){
  ledcWrite(ledChannel_1, 180);
  ledcWrite(ledChannel_2, LOW);
}

void stop_throw_motor(){
  ledcWrite(ledChannel_1, LOW);
  ledcWrite(ledChannel_2, LOW);
}
