
// dispense food
void dispense_food(){
      fd_servo_move(0, max_pos, curr_speed); // open feeder gate 
      wait(2000); // delay 
      fd_servo_move(max_pos, 0, curr_speed); // close feeder gate
}
