
// dispense food
void dispense_food(){
  pot_reading  = analogRead(POT);
  feed_time = pot_map_time(pot_reading);
  fd_servo_move(0, max_pos, curr_speed); // open feeder gate 
  wait(feed_time); // delay 
  fd_servo_move(max_pos, 0, curr_speed); // close feeder gate
}


// throw ball 
void throw_ball(){
  digitalWrite(LED, HIGH); // indicator LED on
  th_servo_move(0, max_pos, curr_speed); // open trapdoor
  dc_motor_routine(); // run motor
  th_servo_move(max_pos, 0, curr_speed); // close trapdoor
  digitalWrite(LED, LOW); // indicator LED off
 }
