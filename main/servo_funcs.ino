// Servo Motor functions ---

// function to move throw servo motor from start_pos to end_pos at speed (speed is time delay between each position step
void th_servo_move(int start_pos, int end_pos, int servo_speed){
  // if start and end position are equal do nothing
  if (start_pos == end_pos){} 
  // if start is less than end step up to end_pos
  else if (start_pos < end_pos) { 
      for (pos = start_pos; pos <= end_pos; pos += 1) {
        th_servo.write(pos);
        delay(servo_speed);
      }
  // if start is more than end step down to end_pos    
  } else { 
      for (pos = start_pos; pos >= end_pos; pos -= 1) {
        th_servo.write(pos);
        delay(servo_speed);
      }
  }
}


// function to move feeder servo motor from start_pos to end_pos at speed (speed is time delay between each position step
void fd_servo_move(int start_pos, int end_pos, int servo_speed){
  // if start and end position are equal do nothing
  if (start_pos == end_pos){} 
  // if start is less than end step up to end_pos
  else if (start_pos < end_pos) { 
      for (pos = start_pos; pos <= end_pos; pos += 1) {
        th_servo.write(pos);
        delay(servo_speed);
      }
  // if start is more than end step down to end_pos    
  } else { 
      for (pos = start_pos; pos >= end_pos; pos -= 1) {
        th_servo.write(pos);
        delay(servo_speed);
      }
  }
}
