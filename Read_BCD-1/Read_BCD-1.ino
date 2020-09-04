/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/DigitalReadSerial
*/

// digital pins 2-5 have a pushbutton attached to it. Give it a name:
int START_DELAY1 = 2;
int START_DELAY2 = 3;
int START_DELAY4 = 4;
int START_DELAY8 = 5;

// digital pins 2-5 have a pushbutton attached to it. Give it a name:
int STOP_DELAY1 = 6;
int STOP_DELAY2 = 7;
int STOP_DELAY4 = 8;
int STOP_DELAY8 = 9;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // Set the BCD's pins inputs:
  pinMode(START_DELAY1, INPUT_PULLUP);
  pinMode(START_DELAY2, INPUT_PULLUP);
  pinMode(START_DELAY4, INPUT_PULLUP);
  pinMode(START_DELAY8, INPUT_PULLUP);
  pinMode(STOP_DELAY1, INPUT_PULLUP);
  pinMode(STOP_DELAY2, INPUT_PULLUP);
  pinMode(STOP_DELAY4, INPUT_PULLUP);
  pinMode(STOP_DELAY8, INPUT_PULLUP);
}


int readStartDelay() {
    // read the input pins:
   unsigned int value = 15; // value from 0 to 15 based on BCD input
     value -= digitalRead(START_DELAY1); // LSB
     value -= 2 * digitalRead(START_DELAY2);
     value -= 4 * digitalRead(START_DELAY4);
     value -= 8 * digitalRead(START_DELAY8); // MSB
    return value;
} 


int readStopDelay() {
    // read the input pins:
   unsigned int value = 15; // value from 0 to 15 based on BCD input
     value -= digitalRead(STOP_DELAY1); // LSB
     value -= 2 * digitalRead(STOP_DELAY2);
     value -= 4 * digitalRead(STOP_DELAY4);
     value -= 8 * digitalRead(STOP_DELAY8); // MSB
    return value;
} 

// the loop routine runs over and over again forever:
void loop() {
  // read the input pins:
   unsigned int value = readStopDelay();

  // print out the state of the BCD:
  Serial.println(value);
  delay(1);        // delay in between reads for stability
}
