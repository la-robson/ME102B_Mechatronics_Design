// Test potentiometer

// Potentiometer should return 1,2,3 depending on position
// This is to be used for controlling the DC motor speeds 


// Setup --------------------------------

#define POT 39  // declare the potentiometer pin number

// define variables
int pot_reading;                  // current potentiometer reading
const int max_pot_reading = 4095; // define maximum potentiometer reading
int output_val;                   // current output value

void setup() {
  // Setup pins
  pinMode(POT, INPUT);

  // set up the serial monitor
  Serial.begin(115200); 
}

// Main loop --------------------------------
void loop() {
  // read value and print in serial port
  pot_reading  = analogRead(POT);
  output_val = pot_map(pot_reading);
  Serial.print("Output = " + String(output_val) + "Pot value = " + String(pot_reading) + '\n');

  delay(1000);
}

// funtion to map the potentiometer value to 1-3
int pot_map(int reading){
  if (reading < max_pot_reading/3) {return 1;}
  else if (reading < max_pot_reading*2/3) {return 2;}
  else {return 3;}
}
