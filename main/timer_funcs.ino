// timer init functions ---
void timer_init(){

  
  // mealtime
  meal_timer = timerBegin(0, 80, true);  // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(meal_timer, onMealtime, true); // edge (not level) triggered
  timerAlarmWrite(meal_timer, mealtime_period, true); // period * 1 us, autoreload true
  timerAlarmEnable(meal_timer); // enable
  timerRestart(meal_timer);
}
