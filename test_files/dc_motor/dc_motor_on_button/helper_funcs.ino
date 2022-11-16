
// get speed
int get_speed(int potCMD, int MAXPWM){
  if (potCMD >= 0 && potCMD <= 60){
      mtrS = MAXPWM*0.9;
  }
  else if (potCMD >= 80 && potCMD <= 140){
    mtrS = MAXPWM*0.97;
  }
  else if (potCMD >= 160 && potCMD <= 220){
    mtrS = MAXPWM;
  }
  return mtrS;
}


void debug_prints(){
  Serial.println("potCMD -- MotorPWM");
  Serial.print(potCMD);
  Serial.print(" -- ");
  Serial.println(mtrS);
  delay(5000);
}
