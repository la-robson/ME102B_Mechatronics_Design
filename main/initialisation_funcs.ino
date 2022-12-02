// Initialisation functions ---


void interrupt_init(){
  attachInterrupt(SWITCH, switch_isr, RISING); 
  attachInterrupt(TH_BTN, throw_button_isr, RISING); 
  attachInterrupt(FD_BTN, feed_button_isr, RISING); 
}

// pin init function 
void pins_init(){
  pinMode(TH_BTN, INPUT);
  pinMode(FD_BTN, INPUT);
  pinMode(SWITCH, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(POT,INPUT);
  pinMode(En_L,OUTPUT);
  pinMode(En_R,OUTPUT);
  th_servo.attach(TH_SEV);
  fd_servo.attach(FD_SEV);
  
}
