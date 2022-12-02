// Misc. helper functions ---


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

// map the potentiometer to the feed drop time for a small medium or big food portion
int pot_map_time(int reading){
  if (reading < max_pot_reading/3) {return small_feed;}
  else if (reading < max_pot_reading*2/3) {return med_feed;}
  else {return large_feed;}
}
