/*

 This example reads all 3 channels of an Analog Devices ADE7913 energy monitor.
 
 The circuit:
 * CS - to digital pin 12  (SS pin)
  */

// inslude the SPI library:
#include <SPI.h>

// set pin 12 as the slave select for the digital pot:
const int slaveSelectPin = 12;

int reading = 0;           // variable to store the value read
long maxVolts = 0;
long maxAmps1 = 0;
long maxAmps2 = 0;
int samples = 200;  // how many samples per reading

void setup() {
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
    Serial1.begin(9600);
  Serial1.println();
  Serial1.println(" -- Startup --");
  Serial1.println();
  Serial1.println();
}

void loop() {
    maxVolts = 0;
    maxAmps1 = 0;
    maxAmps2 = 0;
    
  // sample and keep the maximum value
  for (int counter = 1; counter < samples; counter++) { 
    reading =   readChannel(0x04);  // 0x04 reads channel 0
    if (reading > maxVolts) {
      maxAmps1 = reading;
    }
    delay(1);
    reading =   readChannel(0x0C);  // 0x0C reads channel 1
    if (reading > maxAmps1) {
      maxAmps2 = reading;
    }
    delay(1);
    reading =   readChannel(0x14);  // 0x14 reads channel 2
    if (reading > maxAmps2) {
      maxVolts = reading;
    }
    delay(1);
  }
  
  Serial1.print ("I1= ");
  Serial1.print(maxAmps1); 
  Serial1.print (" I2= ");
  Serial1.print(maxAmps2); 
  Serial1.print (" V= ");
  Serial1.println(maxVolts); 
  
 // delay(200);
}

int readChannel(byte channel) {
  int reading;
  byte read1;
  byte read2;
  byte read3;

  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  //  send in the address and value via SPI:
  //  SPI.transfer(0x04); // which register to reac 0x04 = burst mode
  SPI.transfer(channel); // which register to reac 0x04 = burst mode
 //  delay(1);
  read1 = SPI.transfer(0x00);
  read2 = SPI.transfer(0x00);
  read3 = SPI.transfer(0x00);

  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH); 

  reading = read1;
  reading = (reading << 8) | read2;
return reading;
}

