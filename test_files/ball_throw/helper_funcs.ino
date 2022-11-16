// Misc. functions

// funtion to wait a set period of time using millis
void wait(int wait_time){
  int mini_time = millis();
  while (millis() - mini_time < wait_time){}
}
