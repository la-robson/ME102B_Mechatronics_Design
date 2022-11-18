// DC motor functions ---

void dc_motor_init(){
  // set channels for DC motor
  ledcSetup(ledChan_10, freq, res);
  ledcSetup(ledChan_11, freq, res);
  ledcAttachPin(mtrb1,ledChan_10);
  ledcAttachPin(mtrb2,ledChan_11);
}


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
