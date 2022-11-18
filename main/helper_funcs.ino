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

int pot_map_time(int reading){
  if (reading < max_pot_reading/3) {return 1000;}
  else if (reading < max_pot_reading*2/3) {return 2000;}
  else {return 3000;}
}
