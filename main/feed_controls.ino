/// Feeder ------------

bool feed_start_checker(){
  // if not feeder_state and (mealtime or user_input)
  if (!feeder_state and mealtime){
    return true;
  } else {
    return false;
  }
}

bool throw_feed_count_checker(){
  // (ball_throw_switch and feeder_count < max_feeder_count))
}

void dispense_food_start(){
  feeder_state = true;  // set feeder dispenser state to true (feeding in progress)
  // start timer 
  // open feeder
}

bool food_stop_checker(){
  if (feeder_timer){return true;}
  else {return false;}
}
void dispense_food_stop(){
  // stop and reset timer 

  int start_pos = 0;
  int end_pos = 90;
  int servo_speed = 5;
  servo_move(feeder_servo, start_pos, end_pos, servo_speed);  // close feeder
  feeder_state = false;   // set feeder dispenser state back to false
}
