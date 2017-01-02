/*
This is to control a servo that is attached to the zoom control lever on a Canon Vixia camera.
It adds remote zoom capability, with the ability to zoom in and out at slow and fast speeds.

For demonstration, this code is kept to the most basic of functions.

The serial port is only used for debugging, to show the pot settings and the button activations.

See the video at: https://youtube.com/user/Corgitronics/

More info at:  https://corgitronics.com/

*/

#include <Servo.h>
#include <avr/wdt.h>

Servo myservo;  // create a new servo object

// analog pins
int centerSetpin = 0;  // analog pin for the center setting
int centerVal;         // variable to read the value from the analog pin0

int slowSetpin = 1;  // analog pin for the Slow setting
int slowVal;         // variable to read the value from the analog pin1

int fastSetpin = 2;  // analog pin for the fast setting
int fastVal;         // variable to read the value from the analog pin2

// Buttons
  int FastZoom =10;      // defines the Fast Zoom-in button
  int SlowZoom = 11;     // defines the Slow Zoom-in button
  int SlowWide =7;       // defines the Slow Wide button
  int FastWide = 8;      // defines the Fast Wide button

void setup() {
  Serial.begin(9600);      // for debug, open the serial port at 9600 bps:
  //Buttons
  pinMode(FastZoom, INPUT_PULLUP);      // defines the Fast Zoom-in button
  pinMode(SlowZoom, INPUT_PULLUP);      // defines the Slow Zoom-in button
  pinMode(SlowWide, INPUT_PULLUP);      // defines the Slow Wide button
  pinMode(FastWide, INPUT_PULLUP);      // defines the Fast Wide button

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

// Returns the zoom lever to the neutral (centered) position
void centerServo() {
  centerVal = analogRead(centerSetpin);          // reads the value of the potentiometer (value between 0 and 1023)
  centerVal = map(centerVal, 0, 1023, 0, 40);    // scale it to use it with the servo (value between 0 and 40) 
  centerVal = centerVal +70;                 // Add 70 to the center reading (90 degrees +/- 20) 
  myservo.write(centerVal);                  // sets the servo position according to the scaled value
  delay(15);                                 // waits for the servo to get there
 }

void zoomInFast() {
  int pressed;
  pressed = digitalRead(FastZoom);              // read the button
    fastVal = analogRead(fastSetpin);           // reads the value of the potentiometer (value between 0 and 1023)
    fastVal = map(fastVal, 0, 1023, 0, 30);     // scale it to use it with the servo (value between 0 and 30)
                                                // the FastVal will be up to 30 degrees past the center
  Serial.print("Zoom In Fast  - setting: "); 
  Serial.println(fastVal);
  
   while(!pressed) {                   // hold this servo position as long as the button is pressed
    myservo.write(centerVal + fastVal);       // sets the servo position according to the scaled value
    pressed = digitalRead(FastZoom);          //  read the button
  }
    Serial.println("** centering");
    centerServo();                            // return the zoom lever to the neutral position
}

void zoomInSlow() {
  int pressed;
  pressed = digitalRead(SlowZoom);             // read the button
  Serial.print("Zoom In Slow - setting:");
    slowVal = analogRead(slowSetpin);           // reads the value of the potentiometer (value between 0 and 1023)
    slowVal = map(slowVal, 0, 1023, 0, 30);     // scale it to use it with the servo (value between 0 and 30)
                                                // the SlowVal will be up to 30 degrees past the center
  Serial.println(slowVal);
  
  while(!pressed) {                   // hold this servo position as long as the button is pressed
    myservo.write(centerVal + slowVal);        // sets the servo position according to the scaled value
    pressed = digitalRead(SlowZoom);           //  read the button
  }
    Serial.println("** centering");
    centerServo();                            // return the zoom lever to the neutral position
}

void zoomOutSlow() {
  int pressed;
  pressed = digitalRead(SlowWide);             // read the button
  Serial.print("Zoom out Slow - setting: ");
     slowVal = analogRead(slowSetpin);          // reads the value of the potentiometer (value between 0 and 1023)
    slowVal = map(slowVal, 0, 1023, 0, 30);     // scale it to use it with the servo (value between 0 and 30)
  Serial.println(slowVal);
  
 while(!pressed) {                   // hold this servo position as long as the button is pressed
    myservo.write(centerVal - slowVal);         // sets the servo position according to the scaled value
    pressed = digitalRead(SlowWide);            //  read the button
  }
    Serial.println("** centering");
    centerServo();                            // return the zoom lever to the neutral position
}

void zoomOutFast() {
  int pressed;
  pressed = digitalRead(FastWide);             // read the button
  Serial.println("Zoom Out Fast - setting: ");
    fastVal = analogRead(fastSetpin);           // reads the value of the potentiometer (value between 0 and 1023)
    fastVal = map(fastVal, 0, 1023, 0, 30);     // scale it to use it with the servo (value between 0 and 30)
  Serial.println(fastVal);
  
  while(!pressed) {                   // hold this servo position as long as the button is pressed
    myservo.write(centerVal - fastVal);        // sets the servo position according to the scaled value
    pressed = digitalRead(FastWide);           //  read the button
  }
    Serial.println("** centering");
    centerServo();                            // return the zoom lever to the neutral position
}

// The main control loop
//   look for a button to be pressed
//   then re-center the servo
//   a short pause for stability
void loop() {
  if (!digitalRead(FastZoom)) {
    zoomInFast();
  }
 
  if (!digitalRead(SlowZoom)) {
    zoomInSlow();
  }
  
    if (!digitalRead(SlowWide)) {
    zoomOutSlow();
  }
      if (!digitalRead(FastWide)) {
    zoomOutFast();
  }
  
  centerServo();
  delay(100);                           // pause for stability
  wdt_reset();
}
