
// function to move motor from start_pos to end_pos at speed (speed is time delay between each position step
void servo_move(Servo myservo, int start_pos, int end_pos, int servo_speed){
  int pos;
  // if start and end position are equal do nothing
  if (start_pos == end_pos){} 
  // if start is less than end step up to end_pos
  else if (start_pos < end_pos) { 
      for (pos = start_pos; pos <= end_pos; pos += 1) {
        myservo.write(pos);
        delay(servo_speed);
      }
  // if start is more than end step down to end_pos    
  } else { 
      for (pos = start_pos; pos >= end_pos; pos -= 1) {
        myservo.write(pos);
        delay(servo_speed);
      }
  }
}
