/*  Arduino DC Motor Control - PWM | H-Bridge | L298N  -  Example 01

    by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#define enA 11
#define in1 18
#define in2 19
#define opto 3
#define alarm 4
#define buzzer 5
#define relayOutput 6

#define photoSensor 2

bool rotation = false;
int rotationCount = 0;
int rotationMax = 55;  // how many times through the loop with no rotations to indicate a jammed rotor
   // 10 is too many for high speed and not enough for low speed

int rotationLimit = 0;  // a dynamic limit that is based upon the speed setting of the potentiometer

int potValue = 0;
int forwardSpeed = 0;

#define pot A0

int reverseTime = 1000;

bool jammed = false;
int jammedCount = 0;
int reverseSpeed = 45;

bool runSignal = true;

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
  analogWrite(enA, 1000); // apply braking pulse
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
  digitalWrite(relayOutput, HIGH);
  
  pinMode(opto, INPUT_PULLUP);
  pinMode(photoSensor, INPUT_PULLUP);
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
