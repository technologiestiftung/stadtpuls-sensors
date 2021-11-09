#include <Arduino.h>

int SOUND_SENSOR_PIN = 36;
unsigned int sample;
const int sampleWindow = 500; // Sample window width in mS (50 mS = 20Hz)

void setup() {
  Serial.begin(115200);
}

void loop(){
  sample = analogRead(SOUND_SENSOR_PIN);                //get reading from mi crophon
    unsigned long startMillis = millis();                   // Start of sample window
  float peakToPeak = 0;                                   // peak-to-peak level

  unsigned int signalMax = 0;                             //minimum value
  unsigned int signalMin = 4095;                          //maximum value
                                                          // collect data for 50 mS
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(SOUND_SENSOR_PIN);                //get reading from microphone

    if (sample < 4095) {                                  // toss out spurious readings
         if (sample > signalMax) {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin) {
            signalMin = sample;                           // save just the min levels
         }
      }
  }

  peakToPeak = signalMax - signalMin;                     // max - min = peak-peak amplitude
    Serial.println(peakToPeak);

}