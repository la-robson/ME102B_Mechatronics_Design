// initialise dc throw motor ---------------

void throw_init(){
  
}



/// Ball launcher ------------

bool ball_throw_start_checker(){
  // if not ball_throw_state and (user button press or playtime timer)
  return true;
  // else
  return false;
}

void ball_throw_start(){
  // start timer
  // start rollers
  // open trapdoor
}

bool ball_throw_stop_checker(){
  // if ball_throw_timer
}

void ball_throw_stop(){
  // stop and reset timer
  ball_throw_timer = false; // reset timer flag 
  // stop rollers
  // close trapdoor
}

bool thrower_idle_checker(){
  if (ball_throw_switch) {return true;}
  else {return false;}
}

void thrower_idle(){
  ball_throw_state = false; // set ball launch state flag to false
}
