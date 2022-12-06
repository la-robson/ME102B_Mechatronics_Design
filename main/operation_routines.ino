// Operation routines ---


// dispense food
void dispense_food(){
  // get portion size
  if (mealtime == 2) {
    feed_time = large_feed;
  } else {
    pot_reading  = analogRead(POT);
    feed_time = pot_map_time(pot_reading);
  }
  // dispense food
  fd_servo_move(0, 25, curr_speed); // open feeder gate 
  wait(feed_time); // delay 
  fd_servo_move(25, 0, curr_speed); // close feeder gate
}


// throw ball 
void throw_ball(){
  
  digitalWrite(LED, HIGH); // indicator LED on
  th_servo_move(90, 0, curr_speed); // open trapdoor
  dc_motor_routine(); // run motor
  th_servo_move(0, 90, curr_speed); // close trapdoor
  digitalWrite(LED, LOW); // indicator LED off
  ball_returned = false;    // set ball return to false
 }


// reset after ball returned
void throw_ball_reset(){
  throwButtonPressed = false;
  ball_returned = true;
  switchPressed = false; 
}


// move to treat dispensing state, increment counter and change state
void to_treat_state(){
  feed_count = feed_count + 1;
  state = 4;
}


// move to treat dispensing state, dispense 'meal' and reset the feed counter
void to_mealtime(){
  mealtime = 2;   // update mealtime flag to indicate that a meal is about to be delivered
  feed_count = 0;   // reset feed counter
  state = 4;
}


// move to ide state, reset flags
void to_idle_state(){
  feedButtonPressed = false;
  throwButtonPressed = false;
  switchPressed = false;
  if (mealtime == 2){mealtime = 0;}
  state = 1;
}


// go to wait for ball return
void to_wait_4ball(){
  switchPressed = false; 
  state = 3;
}
