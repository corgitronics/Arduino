/*  Arduino DC Motor Control - PWM | H-Bridge | L298N  -  Example 01

    by Dejan Nedelkovski, www.HowToMechatronics.com
*/

#define enA 11
#define in1 18
#define in2 19
#define opto 21

#define reverseButton 2

int sampleCount = 5;

#define pot A0
#define shunt A3
int currentMax = 600;
int currentMin = 0;
int reverseTime = 2000;

bool jammed = false;
int reverseSpeed = 35;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(reverseButton, INPUT_PULLUP);

  analogReference(INTERNAL);

  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  Serial.begin(9600); // open the serial port at 9600 bps:
  
  //stuff for testing 
//  digitalWrite(in1, HIGH);
//  digitalWrite(in2, LOW);
//  analogWrite(enA, 128); // Send PWM signal to L298N Enable pin
}

void loop() {
  int potValue = analogRead(pot); // Read potentiometer value
  int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
  analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
    float currentValue = 0.0;


  // check for a jammed case

  for (int i = 0; i <= sampleCount; i++){
    currentValue = currentValue + analogRead(shunt); // Read the current shunt value
    delay(5);
  }
  currentValue = currentValue / sampleCount;
  if ((currentValue > currentMax) or (currentValue < currentMin)) {
    delay(100);
    currentValue = analogRead(shunt); // Read the current shunt value
    if ((currentValue > currentMax) or (currentValue < currentMin)) {
    jammed = true;
  }}

  jammed = digitalRead(reverseButton);
      
  Serial.print("jammed: ");
  Serial.print(jammed);
  Serial.print(" currentValue: ");
  Serial.print(currentValue);
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
