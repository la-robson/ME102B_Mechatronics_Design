
// funtion to map the potentiometer value to low med high _speed
int pot_map(int reading){
  if (reading < max_pot_reading/3) {return low_speed;}
  else if (reading < max_pot_reading*2/3) {return med_speed;}
  else {return high_speed;}
}
