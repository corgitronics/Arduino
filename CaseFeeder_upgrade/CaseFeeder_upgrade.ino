/*  Arduino DC Motor Control - PWM | H-Bridge | L298N  -  Example 01

    by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#define enA 11
#define in1 18
#define in2 19
#define opto 21

#define photoSensor 2

bool rotation = false;
int rotationCount = 0;
int rotationMax = 10;

#define pot A0

int reverseTime = 2000;

bool jammed = false;
int reverseSpeed = 35;

void rotationSet(){ // if the photoSensor was triggered then set the rotation flag
  rotation = true;
  Serial.println("* Pulse!");
}

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(photoSensor, INPUT);
  attachInterrupt(0, rotationSet, FALLING);

  analogReference(INTERNAL);

  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  Serial.begin(9600); // open the serial port at 9600 bps:

}

void loop() {
  int potValue = analogRead(pot); // Read potentiometer value
  int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
  analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
    float currentValue = 0.0;


  // check for a jammed case

  if (rotation) {
    rotation = false;
    rotationCount = 0;
  } else  {
    rotationCount++;
  }
  if (rotationCount > rotationMax) {
    jammed = true;
    rotationCount = 0;
  }
        
  Serial.print("jammed: ");
  Serial.print(jammed);

  Serial.print("   potValue: ");
  Serial.println(potValue);

  // If jammed - change rotation direction
  if (jammed == true) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    delay(20);
    analogWrite(enA, reverseSpeed); // Send PWM signal to L298N Enable pin
    delay(reverseTime);  // reversing...
    jammed = false;

  // now reset to forward direction
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
    delay(20);
    }
    delay(100);
}
