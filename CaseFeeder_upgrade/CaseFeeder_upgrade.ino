/*  Motor control for a Dillon Casefeeder
 *   
 *   Uses sensors to detect jams of the feeder and attempts to clear the jam or shutdown if necessary.
 *   
 *   -- Devices
 *   Potentiometer allows user to set the rotation speed of the feeder
 *   Photo sensor to detect rotation of the case feeder shell plate
 *   IR reflective sensor to detect cases stuck in the drop tube
 *   LEDs to indicate motor behavior (green and yellow)
 *   Red LED to indicate the feeder is stuck and needs manual attention
 *   Buzzer to alert user to the alarm
 *   
 *   -- External machine interface
 *   1/8" headphone style socket
 *   Tip - system ground
 *   Ring - Input (3 - 24 VDC) allowing the rotation to be turned on/off by another machine 
 *   Shield - Dry contact output indicating the system is jammed
 *   
 *   -- Principle of operation:
 *   System reads the value of the potentiometer to determine desired speed.
 *   System sets the motor to forward rotation and initiates PWM output for the speed.
 *   System detects pulses from the photodiode indicating that the shell plate is rotating
 *   If the system does not detect photo pulses, it will reverse the rotation for a moment to clear the jam, and then return to normal forward operation
 *   If the system detects multiple consecutive jams, it will sound an alarm and stop the motor.  User must cycle the power switch to reset.
 *   To detect jams more quickly, the rate that photo pulses must arrive varies based upon the selected speed.
 *   
*/

#define pot A0         // for setting speed
#define enA 11         // PWM output to H-Bridge
#define in1 18         // direction setting for H-Bridge
#define in2 19         // direction setting for H-Bridge
#define opto 3         // Opto-isolator for external control input
#define alarm 4        // Red LED for alarm indication
#define buzzer 5       // Sounder for alarm
#define relayOutput 6  // Provides alarm indication to external system

#define photoSensor 2  // for disk rotation
#define irSensor 7     // for droptube funnel

bool rotation = false; // indicates if the interrupt has detected a photo pulse during the last cycle through the loop
int rotationCount = 0; // how many times through the loop since the last photo pulse
int rotationMax = 55;  // how many times through the loop with no rotations to indicate a jammed rotor

int rotationLimit = 0;  // a dynamic limit that is based upon the speed setting of the potentiometer

int potValue = 0;       // current reading from the speed control pot
int forwardSpeed = 0;   // dynamic value based on potValue

int reverseSpeed = 45;  // PWM value to send to the H-Bridge when reversing
int reverseTime = 1000; // milli-seconds to reverse the motor when it jams

bool jammed = false;    // is the motor currently jammed
int jammedCount = 0;    // how many consecutive jams have been counted
int irSensorCount = 0;  // how many loops through that the IR sensor has detected a case in the droptube
int irSensorMax   = 20;  // max number of IR sensor counts until the alarm is triggered

bool runSignal = true;  // controls whether the motor should be running

void rotationSet(){ // if the photoSensor was triggered then set the rotation flag
  rotation = true;
  Serial.println("* Pulse!");
}

void startMotor(int aSpeed){  // this will brake the motor
  analogWrite(enA, 70); // apply starting pulse
  delay(10);
  analogWrite(enA, aSpeed); // set motor speed
}


void stopMotor(){  // this will brake the motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 200); // apply braking pulse
  delay(500);
  analogWrite(enA, 0); // shutoff motor
}

void forwardDirection(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void reverseDirection(){
      digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
}

void signalAlarm(){  // NOTE:  this is an END STATE, once it enters this it never leaves
  int buzzerCount = 0;
  digitalWrite(relayOutput, LOW);
  while (true){  // loop until device is reset
    digitalWrite(alarm, HIGH);
    if (buzzerCount < 3) {  // only buzz N times
      digitalWrite(buzzer, HIGH);
    }
    delay(500);  
    digitalWrite(alarm, LOW);
    digitalWrite(buzzer, LOW);
    delay(500); 
    buzzerCount++;
  }
}

bool checkIRsensor(){  // read the sensor and return whether to shutdown
  bool result = false;
  if (!digitalRead(irSensor)) {
    irSensorCount++;
  } else {
    irSensorCount = 0;  // if nothing in the droptube, then reset the count
  }
  if (irSensorCount > irSensorMax){
    result = true;
  }
  return result;
}

void setup() {
  // TCCR2B = TCCR2B & B11111000 | B00000010; // for PWM frequency of 3921.16 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000011; // for PWM frequency of 980.39 Hz
  //TCCR2B = TCCR2B & B11111000 | B00000101; // for PWM frequency of 245.10 Hz
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(alarm, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relayOutput, OUTPUT);
  digitalWrite(relayOutput, HIGH);  // turn off the alarm relay (uses inverted logic)
  
  pinMode(opto, INPUT_PULLUP);
  pinMode(photoSensor, INPUT_PULLUP);
  pinMode(irSensor, INPUT);
  
  attachInterrupt(0, rotationSet, FALLING);

  analogReference(INTERNAL);

  // Set initial rotation direction
  forwardDirection();
  Serial.begin(9600); // open the serial port at 9600 bps:

}

void loop() {
    Serial.print("   opto: ");
    Serial.println(digitalRead(opto));
  if (digitalRead(opto)) {
    runSignal = false;
  } else {
    runSignal = true;
  }

  if (runSignal){  // only do this stuff if the motor should be running
    // set the motor speed
    potValue = analogRead(pot); // Read potentiometer value
    forwardSpeed = map(potValue, 0, 1023, 5 , 255); // Map the potentiometer value from 0 to 255
    startMotor(forwardSpeed); // Send PWM signal to L298N Enable pin
    forwardDirection();

    rotationLimit = map(potValue, 0, 1023, rotationMax , 10); // Map the potentiometer value to the rotationLimit
  
    // check for a jammed case
    if (rotation) {
      rotation = false;
      rotationCount = 0;
      if (jammedCount > 0){
        jammedCount--;  // decrement the number of consecutive jammed conditions
      }
    } else  {
      rotationCount++;
    }
    if (rotationCount > rotationLimit) {  // check the number of cycles since the last rotation pulse
      jammed = true;
      jammedCount = jammedCount +4;
      rotationCount = 0;   // clear the rotation count
    }

    if (checkIRsensor()) {
      stopMotor();
      signalAlarm();
    }
    Serial.print("jammedCount: ");
    Serial.print(jammedCount);
  
    Serial.print("   potValue: ");
    Serial.println(potValue);
    
    if (jammedCount > 5) {
      analogWrite(enA, 0); // Stop the motor
      Serial.println(" * * * multiple jams - stopping * * *");
      signalAlarm();
    }
    
    // If jammed - change rotation direction
    if (jammed == true) {
      stopMotor();
      reverseDirection();
      delay(20);
      startMotor(reverseSpeed); // Send PWM signal to L298N Enable pin
      delay(reverseTime);  // reversing...
      jammed = false;
  
    // now reset to forward direction
      stopMotor();
      forwardDirection();
      startMotor(forwardSpeed); // Send PWM signal to L298N Enable pin
      delay(20);
      }
      delay(20);
    } else {
      analogWrite(enA, 0); // Stop the motor
      stopMotor();
    }
}
