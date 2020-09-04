int Ipin = 2;
int LED = 9;
int value;

void resetLED()
  {
   digitalWrite(LED, HIGH);
  }
  
void setup() {
  // put your setup code here, to run once:
  pinMode(Ipin, INPUT_PULLUP);
  attachInterrupt(0, resetLED, FALLING);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 delay(1000);                       // wait for a second
  digitalWrite(LED, LOW);
  value = digitalRead(Ipin);
  Serial.print("Pin: ");
  Serial.println(value);
}
