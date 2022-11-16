// DC Motor triggered by limit switch


//Define constants ------------------------------------
#define pot 14
#define SWTCH 21  // check this matches hardware
#define mtrb1 25
#define mtrb2 26
#define LED 13


//Setup variables ------------------------------------
// basic variables
int state = 0;

// Pwm variables
const int freq = 5000;
const int ledChan_0 = 0;
const int ledChan_1 = 1;
const int res = 8;
const int MAXPWM = 255;

// DC Motor variables
int mtrS = 0;

// potentiometer variables
int potRead = 0;
int potCMD = 0;

// switch variables and isr
volatile bool switchPressed = false;
void IRAM_ATTR switch_isr() {  // the function to be called when interrupt is triggered
  switchPressed = true; 
}

//Initialization ------------------------------------
void setup() {
  // set button and LED pins
  pinMode(SWTCH, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(pot,INPUT);

  // attatch interrupt to switch
  attachInterrupt(SWTCH, switch_isr, RISING); 

  // set channels for DC motor
  ledcSetup(ledChan_0, freq, res); // channel 0 setup
  ledcSetup(ledChan_1, freq, res); // channel 1 setup
  ledcAttachPin(mtrb1,ledChan_0);
  ledcAttachPin(mtrb2,ledChan_1);
  
  Serial.begin(115200);
}

// main loop ---------------------------------------
void loop() {

  switch (state){
    // idle
    case 0:
      if (switchPressed) {state = 1;}
      Serial.println("in state 0");
      break;

    // motor running
    case 1:
      Serial.println("in state 1");
      
      // set speed -- motor currently only functions with max pwm 
      potRead = analogRead(pot);
      potCMD = map(potRead, 0, 4095, 0, MAXPWM);
      mtrS = MAXPWM;
      
      // start 
      digitalWrite(LED, HIGH);
      motor_start();
      // run for 2s
      int mini_time = millis();
      while (millis() - mini_time < 2000){}
      // stop
      digitalWrite(LED, LOW);
      motor_stop();

      debug_prints();
      
      // return to idle 
      switchPressed = false; 
      state = 0;
      
      break;
    
  }
  
}


// define functions -------------------


// start motor
void motor_start(){
  ledcWrite(ledChan_0,mtrS);
  ledcWrite(ledChan_1,LOW);
}

// stop motor
void motor_stop(){
  ledcWrite(ledChan_0,LOW);
  ledcWrite(ledChan_1,LOW);
}

// funtion to wait a set period of time using millis
void wait(int wait_time){
  int mini_time = millis();
  while (millis() - mini_time < wait_time){}
}
