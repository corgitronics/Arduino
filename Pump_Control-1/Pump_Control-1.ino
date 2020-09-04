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

int SENSOR = 12;  // sensor input
int PUMP = 13;    // pump relay output

bool SENSOR_PREVIOUS = false;

bool PUMP_RUNNING = false;
int LED_START_DELAY = 10;
int LED_STOP_DELAY = 11;

bool LED_ON = LOW;
bool LED_OFF = HIGH;

bool WET = false;
bool DRY = true;

unsigned long millisPerMinute = 60000;


unsigned long readStartDelay() {
    // read the input pins:
   unsigned int value = 15; // value from 0 to 15 based on BCD input
     value -= digitalRead(START_DELAY1); // LSB
     value -= 2 * digitalRead(START_DELAY2);
     value -= 4 * digitalRead(START_DELAY4);
     value -= 8 * digitalRead(START_DELAY8); // MSB
     Serial.print("StartDelay:  ");
     Serial.println(value);
     Serial.flush();
    return value;
} 

unsigned long readStopDelay() {
    // read the input pins:
   unsigned int value = 15; // value from 0 to 15 based on BCD input
     value -= digitalRead(STOP_DELAY1); // LSB
     value -= 2 * digitalRead(STOP_DELAY2);
     value -= 4 * digitalRead(STOP_DELAY4);
     value -= 8 * digitalRead(STOP_DELAY8); // MSB
     Serial.print("StopDelay:  ");
     Serial.println(value);
     Serial.flush();
     return value;
} 

void startPump() {
    Serial.println("start pump...");
    PUMP_RUNNING = true;
    digitalWrite(PUMP, LED_ON); 
}

void stopPump() {
    Serial.println("stop pump...");
    PUMP_RUNNING = false;
    digitalWrite(PUMP, LED_OFF); 
}


// ** TODO - change multiplier to 60000 milli-seconds per minute
void startTimer() {
  digitalWrite(LED_START_DELAY, LED_ON); 
  unsigned long value = readStartDelay() * millisPerMinute; // multiply by milliseconds in one minute
  delay(value);
  digitalWrite(LED_START_DELAY, LED_OFF); 
  if ( digitalRead(SENSOR) == WET) {
    startPump(); 
  } else {
    SENSOR_PREVIOUS =  digitalRead(SENSOR); 
  }
}

// ** TODO - change multiplier to 60000 milli-seconds per minute
void stopTimer() {
  digitalWrite(LED_STOP_DELAY, LED_ON); 
  unsigned long value = readStopDelay() * millisPerMinute; // multiply by milliseconds in one minute
  delay(value);
  digitalWrite(LED_STOP_DELAY, LED_OFF); 
  if ( digitalRead(SENSOR)== DRY) {
    stopPump(); 
  } else {
    SENSOR_PREVIOUS =  digitalRead(SENSOR); 
  }
}

void controlPump(){
     SENSOR_PREVIOUS =  digitalRead(SENSOR);
     Serial.print("Sensor: ");
     Serial.println(digitalRead(SENSOR));
     
     if  (digitalRead(SENSOR) == WET) {
      if (!PUMP_RUNNING) {
        startTimer();
      } 
     } else {
      if (PUMP_RUNNING) {
        stopTimer();
      }
     }
}

// the setup routine runs once when you press reset:
void setup() {
  digitalWrite(PUMP, LED_OFF);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  pinMode(SENSOR, INPUT);
  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW); 
  
  pinMode(LED_START_DELAY, OUTPUT);
  pinMode(LED_STOP_DELAY, OUTPUT);
  digitalWrite(LED_START_DELAY, HIGH); 
  digitalWrite(LED_STOP_DELAY, HIGH); 
  
  // Set the BCD's pins inputs:
  pinMode(START_DELAY1, INPUT_PULLUP);
  pinMode(START_DELAY2, INPUT_PULLUP);
  pinMode(START_DELAY4, INPUT_PULLUP);
  pinMode(START_DELAY8, INPUT_PULLUP);
  pinMode(STOP_DELAY1, INPUT_PULLUP);
  pinMode(STOP_DELAY2, INPUT_PULLUP);
  pinMode(STOP_DELAY4, INPUT_PULLUP);
  pinMode(STOP_DELAY8, INPUT_PULLUP);

  digitalWrite(LED_START_DELAY, LED_OFF); 
  digitalWrite(LED_STOP_DELAY, LED_OFF); 
  stopPump();
  controlPump();

}



// the loop routine runs over and over again forever:
void loop() {
 // Check the sensor status
  if (digitalRead(SENSOR) != SENSOR_PREVIOUS) {
    controlPump();
  }

  delay(1000);        // delay in between reads for stability
  
}
