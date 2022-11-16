/* Input pins for PWM openloop control */
#define pot 14
#define mtrb1 25
#define mtrb2 26
#define led 13

/*Set up our pwm variables */
const int freq = 5000;
const int ledChan_0 = 0;
const int ledChan_1 = 1;
const int ledChan_2 = 2;
const int res = 8;
const int MAXPWM = 255;

/*Variables I wanna handle in my script */
int mtrS = 0;
int potRead = 0;
int potCMD = 0;
int state = 1;

/*timer stuff*/
volatile bool flagMTR = false;
hw_timer_t*timer0 = NULL;
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED; 

void IRAM_ATTR StopMTR() {
  portENTER_CRITICAL_ISR(&timerMux0);
  flagMTR = true; // the function to be called when timer interrupt is triggered
  portEXIT_CRITICAL_ISR(&timerMux0);
  timerStop(timer0);
  timerRestart(timer0);
}

void TimerInterruptInit(){
  timer0 = timerBegin(0, 80, true); //1,000,000 tics = 1 sec
  timerAttachInterrupt(timer0,StopMTR,true);
  timerAlarmWrite(timer0,5000000,true); //how long we wait to trigger the interrupt (5 sec)
  timerAlarmEnable(timer0);
}


void setup() {
  pinMode(pot,INPUT);
  Serial.begin(115200);
  
  ledcSetup(ledChan_0, freq, res); // channel 0 setup
  ledcSetup(ledChan_1, freq, res); // channel 1 setup
  ledcSetup(ledChan_2, freq, res);
  
  ledcAttachPin(mtrb1,ledChan_0);
  ledcAttachPin(mtrb2,ledChan_1);
  ledcAttachPin(led, ledChan_2);
  
  TimerInterruptInit();
}

// main loop ---------------------------------------
void loop() {
  // get value from potentiometer
   potRead = analogRead(pot);
   potCMD = map(potRead,0, 4095, 0, MAXPWM);
   
  /*Constantly running open loop, could probably translate this to switch cases*/
  if (potCMD >= 0 && potCMD <= 60){
      state = 1;
      mtrS = MAXPWM*0.9;
  }
  else if (potCMD >= 80 && potCMD <= 140){
    state = 2;
    mtrS = MAXPWM*0.97;
  }
  else if (potCMD >= 160 && potCMD <= 220){
    state = 3;
    mtrS = MAXPWM;
  }

   /*don't use delay, in an interrupt, it will spit out an error*/
   GO();
   ledcWrite(ledChan_0,mtrS);
   ledcWrite(ledChan_1,LOW);
   ledcWrite(ledChan_2,LOW); // led off
   
   /*Sloppy event checker debugger*/
   if (flagMTR == true){
     int mini_time = millis();
     while (millis() - mini_time < 2000){
        ledcWrite(ledChan_2,HIGH); // LED on 
        ledcWrite(ledChan_0,LOW);
        ledcWrite(ledChan_1,LOW);
    }
   }
   
  /*Handling Interrupt Stuff*/
  portENTER_CRITICAL_ISR(&timerMux0);
  flagMTR = false; // reset our interrupt flag
  portEXIT_CRITICAL_ISR(&timerMux0);
    
  /*Debug Prints EWW*/
  Serial.println("potCMD -- MotorPWM");
  Serial.print(potCMD);
  Serial.print(" -- ");
  Serial.println(mtrS);
  
}



// define functions -------------------

// start timer
void GO(){
  timerStart(timer0);
}
