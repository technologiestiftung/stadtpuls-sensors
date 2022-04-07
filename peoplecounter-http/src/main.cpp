#include <Arduino.h> 
#include <heltec.h> // Library for OLED and Board

//Sensor IN
const int trigIN = 12;
const int echoIN = 13;

//Sensor OUT
const int trigOUT = 32;
const int echoOUT = 33;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

int currentPeople = 0;
int timeoutCounter = 0;
String sequence = "";
int sensorINdistance;
int sensorOUTdistance;
long duration;
float distanceCm;

//display function
void displayMeasurement() {
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->drawString(0,20,"Personen im Haus: ");
  Heltec.display->setFont(ArialMT_Plain_16);  Heltec.display->drawString(0,32,String(currentPeople));
}

int distanz(int trig, int echo) {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
  
    // Reads echoPin, returns the sound wave travel time (to object AND back) in microseconds
    duration = pulseIn(echo, HIGH);
    distanceCm = duration * SOUND_SPEED/2;
    
    return distanceCm;
}

/*
// average of three measurements
int distanzMittel(int trig, int echo) {
  byte count=3;
  int distance=0;
  int sum = 0;
  for (int i = 0; i < count; i++){
        distance=distanz(trig, echo);
        delayMicroseconds(2);
        sum=(sum+distance);
  }
  distance = sum/count;
  return distance;
}
*/

void setup() {
  // display stuff
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->flipScreenVertically();

  //Serial.begin(115200); // Starts the serial communication
  pinMode(trigIN, OUTPUT); 
  pinMode(echoIN, INPUT); 
  pinMode(trigOUT, OUTPUT); 
  pinMode(echoOUT, INPUT);
  delay(500);
  // this is my Eingangsbereich
  sensorINdistance = distanz(trigIN, echoIN);
  sensorOUTdistance = distanz(trigOUT, echoOUT);

}

void loop() {
  int distanceIN = distanz(trigIN, echoIN);
  Serial.print("Distance IN (cm): ");
  Serial.println(distanceIN);

  int distanceOUT = distanz(trigOUT, echoOUT);
  Serial.print("Distance OUT (cm): ");
  Serial.println(distanceOUT);

  if(distanceIN < 10 && sequence.charAt(0) != '1'){
  sequence += "1";
  }
  else if(distanceOUT < 10 && sequence.charAt(0) != '2'){
  sequence += "2";
  }

  if(sequence.equals("12")){
    currentPeople++;  
    sequence="";
    delay(550);
  }
  else if(sequence.equals("21") && currentPeople > 0){
  currentPeople--;  
  sequence="";
  delay(550);
  }

  //Resets the sequence if it is invalid or timeouts
  if(sequence.length() > 2 || sequence.equals("11") || sequence.equals("22") || timeoutCounter > 200){
  sequence="";  
  }

  if(sequence.length() == 1){ //
    timeoutCounter++;
  }
  else{
    timeoutCounter=0;
  }

  // clear the display
  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->drawString(0,10,"Personen im Haus: ");
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(60,30,String(currentPeople));
  Heltec.display->display();

  Serial.print("Anzahl Personen: ");
  Serial.println(currentPeople); 

  delay(50);
}
