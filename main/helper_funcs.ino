// Misc. functions


// funtion to wait a set period of time using millis
void wait(int wait_time){
  int mini_time = millis();
  while (millis() - mini_time < wait_time){}
}

// flash indicator LED
void flash_LED(){
  digitalWrite(LED, HIGH);
  wait(100);
  digitalWrite(LED, LOW);
  wait(100);
}


// light LED corresponding to pot value
void pot_led_indicator(){
   pot_reading  = pot_map_time(analogRead(POT));
  
  if (pot_reading == large_feed){ 
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    }
  else if (pot_reading == med_feed){
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    }
  else if (pot_reading == small_feed){
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    }

}

int pot_map_time(int reading){
  if (reading < max_pot_reading/3) {return small_feed;}
  else if (reading < max_pot_reading*2/3) {return med_feed;}
  else {return large_feed;}
}
