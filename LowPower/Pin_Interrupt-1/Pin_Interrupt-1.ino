/*
* Simple pin driven interrupt, the arduino wakes up when there is a state change on PIN 2.
* 
* Uses about 19 uA with the switch open and about 22 uA with the switch closed.
*
* Requires proper pull-up and pull-down resistors to reduce current draw.
*
* 
* Resistors needed:
*   Pin2 --> 1 Meg Ohm --> GND
*   Pin2 --> button --> 220K Ohm --> Vcc
*  
* Connect an LED to pin 13, use a 330 Ohm resistor, or whatever is needed for your LED.
*/


#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
//#include <sleep_bod_disable.h>
#include <avr/io.h>
#include <avr/wdt.h>

#define adc_disable()  (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()   (ADCSRA |=  (1<<ADEN)) // re-enable ADC
#define ac_disable()   (ACSR   |=  (1<<ACD )) // disable analogue comparator
#define ac_enable()    (ACSR   &= ~(1<<ACD )) // enable analogue comparator


void setup()
{
  wdt_disable();
     //  Serial.begin(9600);
    pinMode(2, INPUT);
    pinMode(13,OUTPUT);      // set pin 13 as an output so we can use LED to monitor
    digitalWrite(13,HIGH);   // turn pin 13 LED on
}

void loop()
{
    // Stay awake for 1 second, then sleep.
    // LED turns off when sleeping, then back on upon wake.
    delay(2000);
     //   Serial.println("Entering Sleep Mode");
    sleepNow();
     //   Serial.println(" ");
     //   Serial.println("I am now Awake");
}
                
void sleepNow()
{ 
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  adc_disable();
  ac_disable();
  sleep_bod_disable(); // Disable brown out detection during sleep. Saves more power
  
    interrupts();
    // Set pin 2 as interrupt and attach handler:
    attachInterrupt(0, pinInterrupt, HIGH);
    pinMode(13, INPUT); // reduces power consumption during sleep
    
    // Set sleep enable (SE) bit:
    sleep_enable();
    
    // Put the device to sleep:
    digitalWrite(13,LOW);    // turn LED off to indicate sleep
    sleep_mode();
  // Arduino is now sleeping, waiting for interrupt
    
  // Upon waking up, sketch continues from this point.
    sleep_disable();
    pinMode(13, OUTPUT);     // set the pint to output again
    digitalWrite(13,HIGH);   // turn LED on to indicate awake
}

void pinInterrupt()
{
    detachInterrupt(0);
    attachInterrupt(0, pinInterrupt, HIGH);
}

