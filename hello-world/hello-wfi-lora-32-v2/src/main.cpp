#include <Arduino.h>

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello World!");

#if defined(WIFI_LoRa_32_V2)
  Serial.println("WIFI_LoRa_32_V2");
#endif

#if defined(WIFI_Kit_32)
  Serial.println("WIFI_Kit_32");
#endif
}
void loop()
{
}