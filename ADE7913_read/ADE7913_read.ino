/*

 This example controls an Analog Devices ADE7913 energy monitor.
 
 The circuit:
 * CS - to digital pin 12  (SS pin)
  */

// inslude the SPI library:
#include <SPI.h>

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 12;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
}

void loop() {
  // go through the six channels of the digital pot:
  for (int channel = 0; channel < 6; channel++) { 
      readAll();
    // wait a second at the top:
    delay(100);
  }
}

void readAll() {
  byte read1;
  byte read2;
  byte read3;
  
  byte read4;
  byte read5;
  byte read6;
  
  byte read7;
  byte read8;
  byte read9;
  
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(0x04); // which register to reac 0x04 = burst mode
  
  read1 = SPI.transfer(0x00);
  read2 = SPI.transfer(0x00);
  read3 = SPI.transfer(0x00);
  
  read4 = SPI.transfer(0x00);
  read5 = SPI.transfer(0x00);
  read6 = SPI.transfer(0x00);
    
  read7 = SPI.transfer(0x00);
  read8 = SPI.transfer(0x00);
  read9 = SPI.transfer(0x00);

  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH); 

  Serial.print("IWV= ");
  Serial.print(read1);
  Serial.print(" ");
  Serial.print(read2);
  Serial.print(" ");
  Serial.print(read3);
  
    Serial.print("     V1WV= ");
  Serial.print(read4);
  Serial.print(" ");
  Serial.print(read5);
  Serial.print(" ");
  Serial.print(read6);
  
    Serial.print("     V2WV= ");
  Serial.print(read7);
  Serial.print(" ");
  Serial.print(read8);
  Serial.print(" ");
  Serial.println(read9);

}

