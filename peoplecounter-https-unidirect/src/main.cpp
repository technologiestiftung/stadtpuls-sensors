#include <Arduino.h> 
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <env.h>
#include <heltec.h> // Library for OLED and Board

WiFiMulti wifiMulti;
#define WiFi_timeout 10000

int sendingIteration = 0;
String requestString = "";

//Sensor IN
const int trigIN = 12;
const int echoIN = 13;

//Sensor OUT
const int trigOUT = 32;
const int echoOUT = 33;

//button
const int button=2;
int buttonStatus=0;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

int prevPeople = 0;
int currentPeople = 0;
int timeoutCounter = 0;
String sequence = "";
int sensorINdistance;
int sensorOUTdistance;
long duration;
float distanceCm;

// void displayMeasurement() {
//   Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
//   Heltec.display->drawString(0,20,"Personen im LAB: ");
//   Heltec.display->setFont(ArialMT_Plain_16);  Heltec.display->drawString(0,32,String(currentPeople));
// }

int distanz(int trig, int echo) {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    // Reads echoPin, returns the sound wave travel time (to object AND back) in microseconds
    duration = pulseIn(echo, HIGH, 100000);
    distanceCm = duration * SOUND_SPEED/2;
    return distanceCm;
}

//WiFi function
void connectToWifi()
{
  wifiMulti.addAP(ssid0, pass0);
  // wifiMulti.addAP(ssid1, pass1); //for second WIFI

  Serial.println("Connecting to Wifi...");

  if (wifiMulti.run(WiFi_timeout) == WL_CONNECTED)
  {
    Serial.println("\n–––––––––––––––––––––––––––––––");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\n–––––––––––––––––––––––––––––––");
    Serial.println("WiFi not Connected");
  }
}

void sendValuesToStadtpuls()
{
  WiFiClientSecure client;

  client.setCACert(root_ca);

  if (!client.connect(server, 443))
  {
    Serial.println("Connection failed!");
  }
  else
  {
    Serial.println("Connected to server!");
    Serial.println("\n- - - - - - - - - - - - - - - -");
    // Make a HTTP request:
    client.println("POST " + String(path) + " HTTP/1.0");
    client.println("Host: api.stadtpuls.com");
    client.println("Content-Type: application/json");
    client.println("Content-Length: " + String(requestString.length()));
    client.println("Authorization: Bearer " + String(authToken));
    client.println("Connection: close");
    client.println();
    client.println(requestString);

    while (client.connected())
    {
      String line = client.readStringUntil('\n');
      if (line == "\r")
      {
        Serial.println("POST Success!");
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available())
    {
      char c = client.read();
      Serial.write(c);
    }

    Serial.println("\n- - - - - - - - - - - - - - - -");

    client.stop();
  }
}

void updateRequestValue()
{
  requestString = "{\"measurements\":[" + String(currentPeople) + "]}";
  Serial.println(requestString);
}

void setup() {

  connectToWifi();

  // display stuff
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->flipScreenVertically();

  //Serial.begin(115200); // Starts the serial communication
  pinMode(button, INPUT); 
  pinMode(trigIN, OUTPUT); 
  pinMode(echoIN, INPUT); 
  pinMode(trigOUT, OUTPUT); 
  pinMode(echoOUT, INPUT);
  delay(500);
  // this is my Eingangsbereich
  sensorINdistance = distanz(trigIN, echoIN);
  delay(50);
  sensorOUTdistance = distanz(trigOUT, echoOUT);

  // Serial.print("initial IN: ");
  // Serial.println(sensorINdistance); 
  // Serial.print("initial OUT: ");
  // Serial.println(sensorOUTdistance); 
}

void loop() {
  buttonStatus = digitalRead(button); 
  if (buttonStatus == HIGH){
    currentPeople = 0;
  }
  else{}

  int distanceIN = distanz(trigIN, echoIN);
  delay(50); //avoid sonic interference!
  int distanceOUT = distanz(trigOUT, echoOUT);

  if(distanceIN < sensorINdistance - 20 && sequence.charAt(0) != '1'){
    sequence += "1";
  }
  else if(distanceOUT < sensorOUTdistance - 20 && sequence.charAt(0) != '2'){
    sequence += "2";
  }

  if(sequence.equals("1")){
    currentPeople++;  
    sequence="";
  }
  else if(sequence.equals("2") && currentPeople > 0){
    currentPeople--;  
    sequence="";
  }

  //Resets the sequence if it is invalid or timeouts
  if(sequence.length() > 1 || sequence.equals("11") || sequence.equals("22") || timeoutCounter > 20){
  sequence="";  
  }

  if(sequence.length() == 0){ //
    timeoutCounter++;
  }
  else{
    timeoutCounter=0;
  }

  // clear the display
  Heltec.display->clear();
  Heltec.display->setFont(ArialMT_Plain_16);
  Heltec.display->drawString(0,0,"Personen im LAB");
  Heltec.display->drawRect(94, 24, 28, 30); //(x1,y1,width,height)
  Heltec.display->drawString(104,30,String(currentPeople));
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->drawString(0,24,"IN distance: ");
  Heltec.display->drawString(60,24,String(distanceIN));
  Heltec.display->drawString(0,36,"OUT distance: ");
  Heltec.display->drawString(70,36,String(distanceOUT));
  Heltec.display->drawString(0,48,"timeout: ");
  Heltec.display->drawString(60,48,String(timeoutCounter));

  Heltec.display->display();

  Serial.print("Anzahl Personen: ");
  Serial.println(currentPeople); 

  Serial.print("IN dist: ");
  Serial.println(distanceIN); 

  Serial.print("OUT dist: ");
  Serial.println(distanceOUT); 

  if (currentPeople != prevPeople && currentPeople >= 0)
  {
    Serial.println("\n–––––––––––––––––––––––––––––––");
    Serial.println(String("Sending Iteration: " + String(sendingIteration)));
    Serial.println("- - - - - - - - - - - - - - - -\n");

    updateRequestValue();
    sendingIteration += 1;

    if (wifiMulti.run() == WL_CONNECTED)
    {
      if (sendingIteration > 1)
      {
        sendValuesToStadtpuls();
      }
    }
    else
    {
      connectToWifi();
    }

    prevPeople = currentPeople;
    delay(100); //wait
  }
}
