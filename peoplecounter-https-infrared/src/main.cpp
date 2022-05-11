/*
This example shows how to set up and read multiple VL53L1X sensors connected to
the same I2C bus. Each sensor needs to have its XSHUT pin connected to a
different Arduino pin, and you should change sensorCount and the xshutPins array
below to match your setup.
*/

#include <Arduino.h> 
#include <Wire.h>
#include <VL53L1X.h>

// The number of sensors in your system.
const uint8_t sensorCount = 2;

// The Arduino pin connected to the XSHUT pin of each sensor.
const uint8_t xshutPins[sensorCount] = { 4, 5 };

VL53L1X sensors[sensorCount];
int sensorINdistance, sensorOUTdistance, distanceIN, distanceOUT;
int prevPeople = 0;
int currentPeople = 0;
int timeoutCounter = 0;
String sequence = "";

void setup()
{
  while (!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  // Disable/reset all sensors by driving their XSHUT pins low.
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    pinMode(xshutPins[i], OUTPUT);
    digitalWrite(xshutPins[i], LOW);
  }

  // Enable, initialize, and start each sensor, one by one.
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    // Stop driving this sensor's XSHUT low. This should allow the carrier
    // board to pull it high. (We do NOT want to drive XSHUT high since it is
    // not level shifted.) Then wait a bit for the sensor to start up.
    pinMode(xshutPins[i], INPUT);
    delay(10);

    sensors[i].setTimeout(500);
    if (!sensors[i].init())
    {
      Serial.print("Failed to detect and initialize sensor ");
      Serial.println(i);
      while (1);
    }

    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    sensors[i].setAddress(0x2A + i);

    sensors[i].startContinuous(50);
    delay(50);

    /*
    // this is my Eingangsbereich
    sensorINdistance = sensors[0].read();
    delay(50);
    sensorOUTdistance = sensors[1].read();
    delay(50);
    */
  }
}

void loop()
{
    distanceIN = sensors[0].read();
    Serial.print("distanceIN ");
    Serial.println(distanceIN);
    delay(50);

    distanceOUT = sensors[1].read();
    Serial.print("distanceOUT ");
    Serial.println(distanceOUT);
    delay(50);

    if(distanceIN < 2000 - 10  && sequence.charAt(0) != '1'){ // hard coded 2m
    sequence += "1";
      }
      else if(distanceOUT < 2000 - 10 && sequence.charAt(0) != '2'){ // hard coded 2m
        sequence += "2";
      }
    
      if(sequence.equals("12")){
        currentPeople++;  
        sequence="";
      }
      else if(sequence.equals("21") && currentPeople > 0){
        currentPeople--;  
        sequence="";
      }
    
      //Resets the sequence if it is invalid or timeouts
      if(sequence.length() > 2 || sequence.equals("11") || sequence.equals("22") || timeoutCounter > 20){
      sequence="";  
      }
    
      if(sequence.length() == 1){ //
        timeoutCounter++;
      }
      else{
        timeoutCounter=0;
      }
      
    Serial.print("Anzahl Personen: ");
    Serial.println(currentPeople); 
        
    for (uint8_t i = 0; i < sensorCount; i++)
    {
    if (sensors[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
    Serial.print('\t');
    Serial.println();
    }
}