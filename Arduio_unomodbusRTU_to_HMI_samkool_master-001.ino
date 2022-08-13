/*
  Modbus-Arduino Example - Lamp (Modbus Serial)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/
 
#include <Modbus.h>
#include <ModbusSerial.h>
#include "DHT.h"
#define DHTPIN 7  
#define DHTTYPE DHT11   // DHT 11

// Modbus Registers Offsets (0-9999)
const int LAMP1_COIL = 100; 
const int SENSOR_IREG = 100; 
const int HODING_HREG = 100; 
const int DHT_IREG = 110; 


// Used Pins
const int ledPin = 13;
const int Relay = 8;
const int sensorPin = A0;

// ModbusSerial object
ModbusSerial mb;
DHT dht(DHTPIN, DHTTYPE);
long ts;

void setup() {
    // Config Modbus Serial (port, speed, byte format) 
    mb.config(&Serial, 38400, SERIAL_8N1);
    // Set the Slave ID (1-247)
    mb.setSlaveId(10);  
    
    // Set ledPin mode
    pinMode(ledPin, OUTPUT);
    pinMode(Relay, OUTPUT);
    dht.begin();
    // Add LAMP1_COIL register - Use addCoil() for digital outputs
    mb.addCoil(LAMP1_COIL);
    mb.addIreg(SENSOR_IREG);
    mb.addIreg(DHT_IREG);
    mb.addHreg(HODING_HREG, 500);
    ts = millis();
}

void loop() {
   // Call once inside loop() - all magic here
   mb.task();
   
   // Attach ledPin to LAMP1_COIL register     
   digitalWrite(ledPin, mb.Coil(LAMP1_COIL));
   
    //Read each two seconds
   if (millis() > ts + 2000) {   
       ts = millis();
       //Setting raw value (0-1024)
       mb.Ireg(SENSOR_IREG, analogRead(sensorPin));
       mb.Ireg(DHT_IREG, dht.readTemperature());
   } 
   if(mb.Hreg(HODING_HREG) >=500){
      digitalWrite(Relay,LOW);
  }
    else{
     digitalWrite(Relay,HIGH);
  }

}
