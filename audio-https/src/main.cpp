#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <env.h>

WiFiMulti wifiMulti;

#define WiFi_timeout 10000

int sendingPeriod = 10 * 1000;
int sendingIteration = 0;
int measuringPeriod = 100;
int measuringIteration = 0;
unsigned long time_now = 0;

int baseADCVal = 1250;
int valTwoIterationsAgo = 0;
int valPrevIteration = 0;
int valCurrentIteration = 0;
int sumAllHighPeakValues = 0;
int totalHighPeaksCount = 0;
int sumAllLowPeakValues = 0;
int totalLowPeaksCount = 0;
int maxPeak = 0;
int minPeak = 0;
float averageHighPeaks = 0;
float averageLowPeaks = 0;
float totalAverage = 0;
String requestString = "";

void connectToWifi()
{
  wifiMulti.addAP(ssid0, pass0);
  wifiMulti.addAP(ssid1, pass1);

  Serial.println("Connecting Wifi...");

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

void updateValues()
{
  valCurrentIteration = analogRead(SENSOR_PIN);

  if (minPeak == 0)
  {
    minPeak = valCurrentIteration;
  }
  if (maxPeak == 0)
  {
    maxPeak = valCurrentIteration;
  }
  if (valCurrentIteration > maxPeak)
  {
    maxPeak = valCurrentIteration;
  }
  if (valCurrentIteration < minPeak)
  {
    minPeak = valCurrentIteration;
  }
  if (valPrevIteration > valTwoIterationsAgo && valPrevIteration > valCurrentIteration)
  {
    sumAllHighPeakValues += valPrevIteration;
    totalHighPeaksCount += 1;
  }
  if (valPrevIteration < valTwoIterationsAgo && valPrevIteration < valCurrentIteration)
  {
    sumAllLowPeakValues += valPrevIteration;
    totalLowPeaksCount += 1;
  }

  valTwoIterationsAgo = valPrevIteration;
  valPrevIteration = valCurrentIteration;
}

void updateAverages()
{
  if (totalHighPeaksCount > 0)
  {
    averageHighPeaks = sumAllHighPeakValues / totalHighPeaksCount;
  }
  if (totalLowPeaksCount > 0)
  {
    averageLowPeaks = sumAllLowPeakValues / totalLowPeaksCount;
  }
  if (totalHighPeaksCount > 0 && totalLowPeaksCount > 0)
  {
    totalAverage = averageLowPeaks + ((averageHighPeaks - averageLowPeaks) / 2);
  }
}

void resetValues()
{
  sumAllHighPeakValues = 0;
  totalHighPeaksCount = 0;
  sumAllLowPeakValues = 0;
  totalLowPeaksCount = 0;
  maxPeak = 0;
  minPeak = 0;
  averageHighPeaks = 0;
  averageLowPeaks = 0;
}

void updateRequestValue()
{
  requestString = "{\"measurements\":[" + String(averageHighPeaks - baseADCVal) + "," + String(maxPeak - baseADCVal) + "," + String(minPeak - baseADCVal) + "," + String(totalAverage - baseADCVal) + "," + String(averageLowPeaks - baseADCVal) + "]}";
  Serial.println(requestString);
}

void setup()
{
  Serial.begin(115200);
  connectToWifi();
}

void loop()
{
  if (millis() >= sendingIteration * sendingPeriod)
  {
    Serial.println("\n–––––––––––––––––––––––––––––––");
    Serial.println(String("Sending Iteration: " + String(sendingIteration)));
    Serial.println("- - - - - - - - - - - - - - - -\n");

    updateAverages();
    updateRequestValue();
    sendingIteration += 1;

    if (wifiMulti.run() == WL_CONNECTED)
    {
      if (sendingIteration > 1)
      {
        sendValuesToStadtpuls();
        resetValues();
      }
    }
    else
    {
      connectToWifi();
    }
  }

  if (millis() >= measuringIteration * measuringPeriod)
  {
    updateValues();
    measuringIteration += 1;
  }
}
