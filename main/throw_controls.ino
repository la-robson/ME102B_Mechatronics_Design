#include <ESP32Encoder.h>

// define encoder pins
#define BIN_1 26
#define BIN_2 25

// create encoder object
ESP32Encoder encoder;

// define motor control variables ----------------------------
int omegaSpeed = 0;
int omegaDes = 0;
int omegaMax = 19;   // CHANGE THIS VALUE TO YOUR MEASURED MAXIMUM SPEED
int D = 0;
int dir = 1;
int Kp = 5;   // TUNE THESE VALUES TO CHANGE CONTROLLER PERFORMANCE
int Ki = 100;  // Ki value is divided by 100, so Ki=10 is equivelent to 0.1
int IMax = 0;
int error = 0;
int sum_error = 0;

// setting PWM properties ----------------------------
const int freq = 5000;
const int ledChannel_1 = 1;
const int ledChannel_2 = 2;
const int resolution = 8;
const int MAX_PWM_VOLTAGE = 255;
const int NOM_PWM_VOLTAGE = 150;


void throw_init() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  ESP32Encoder::useInternalWeakPullResistors = UP; // Enable the weak pull up resistors
  encoder.attachHalfQuad(33, 27); // Attache pins for use as encoder pins
  encoder.setCount(0);  // set starting count value after attaching

  // configure LED PWM functionalitites
  ledcSetup(ledChannel_1, freq, resolution);
  ledcSetup(ledChannel_2, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(BIN_1, ledChannel_1);
  ledcAttachPin(BIN_2, ledChannel_2);
}



/// Ball launcher ------------

bool ball_throw_start_checker(){
  // if not ball_throw_state and (user button press or playtime timer)
  return true;
  // else
  return false;
}

void ball_throw_start(){
  // start timer
  // start rollers
  // open trapdoor
}

bool ball_throw_stop_checker(){
  // if ball_throw_timer
}

void ball_throw_stop(){
  // stop and reset timer
  ball_throw_timer = false; // reset timer flag 
  // stop rollers
  // close trapdoor
}

bool thrower_idle_checker(){
  if (ball_throw_switch) {return true;}
  else {return false;}
}

void thrower_idle(){
  ball_throw_state = false; // set ball launch state flag to false
}
