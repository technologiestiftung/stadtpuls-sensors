/**
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Copyright (c) 2018 Terry Moore, MCCI
 * Copyright (c) 2021-2022 Julia Zimmermann, Lucas Vogel, Fabian Morón Zirfas, Technologiestiftung Berlin
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "Hello,
 * world!", using frequency and encryption settings matching those of
 * the The Things Network.
 *
 * This uses ABP (Activation-by-personalisation), where a DevAddr and
 * Session keys are preconfigured (unlike OTAA, where a DevEUI and
 * application key is configured, while the DevAddr and session keys are
 * assigned/generated in the over-the-air-activation procedure).
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
 * g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
 * violated by this sketch when left running for longer)!
 *
 * To use this sketch, first register your application and device with
 * the things network, to set or generate a DevAddr, NwkSKey and
 * AppSKey. Each device should have their own unique values for these
 * fields.
 *
 * Do not forget to define the radio type correctly in
 * arduino-lmic/project_config/lmic_project_config.h or from your BOARDS.txt.
 *
 */

#include <Arduino.h>
// +++ LoRaWan TTN library
// the order of these includes has to stay like this
#include <lmic.h>
// keep it like this
#include <hal/hal.h>
// arrrrrrggggghhhhhh!
#include <SPI.h>
// our secrets
#include "env.h"
#include <Wire.h>
#include "Adafruit_MCP9808.h"

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

int measuringPeriod = 2000; // in ms
int measuringIteration = 0;
unsigned long time_now = 0;

float measurement = 0;

int SENSOR_PIN = 36;

void do_send(osjob_t *j);

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in arduino-lmic/project_config/lmic_project_config.h,
// otherwise the linker will complain).
void os_getArtEui(u1_t *buf) {}
void os_getDevEui(u1_t *buf) {}
void os_getDevKey(u1_t *buf) {}

uint8_t tx_payload[8];
static osjob_t sendjob;

void sample_data()
{
  tempsensor.wake(); // wake up, ready to read!
  measurement = tempsensor.readTempC();
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
}

// void updateAverages()
// {
//   if (totalHighPeaksCount > 0)
//   {
//     averageHighPeaks = sumAllHighPeakValues / totalHighPeaksCount;
//   }
//   if (totalLowPeaksCount > 0)
//   {
//     averageLowPeaks = sumAllLowPeakValues / totalLowPeaksCount;
//   }
//   if (totalHighPeaksCount > 0 && totalLowPeaksCount > 0)
//   {
//     totalAverage = averageLowPeaks + ((averageHighPeaks - averageLowPeaks) / 2);
//   }
// }

// void resetValues()
// {
//   sumAllHighPeakValues = 0;
//   totalHighPeaksCount = 0;
//   sumAllLowPeakValues = 0;
//   totalLowPeaksCount = 0;
//   maxPeak = 0;
//   minPeak = 0;
//   averageHighPeaks = 0;
//   averageLowPeaks = 0;
// }

// Schedule TX every this many seconds (might become longer due to duty cycle
// limitations).ssss
const unsigned TX_INTERVAL = 60 * 5; // 5 minutes

//
// PIN MAPPING FOR HELTEC ESP32 V2 --> do not change
//
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 14, // reset pin
    .dio = {26, 34, 35},
};

//
// PAYLOAD HELPER FUNCTION --> do not change
//
void printHex2(unsigned v)
{
  v &= 0xff;
  if (v < 16)
    Serial.print('0');
  Serial.print(v, HEX);
}

// TTN DEBUGGING
// Messages can be checked withing your serial monitor. Do not
// change!
//
void onEvent(ev_t ev)
{
  Serial.print(os_getTime());
  Serial.print(": ");
  switch (ev)
  {
  case EV_SCAN_TIMEOUT:
    Serial.println(F("EV_SCAN_TIMEOUT"));
    break;
  case EV_BEACON_FOUND:
    Serial.println(F("EV_BEACON_FOUND"));
    break;
  case EV_BEACON_MISSED:
    Serial.println(F("EV_BEACON_MISSED"));
    break;
  case EV_BEACON_TRACKED:
    Serial.println(F("EV_BEACON_TRACKED"));
    break;
  case EV_JOINING:
    Serial.println(F("EV_JOINING"));
    break;
  case EV_JOINED:
    Serial.println(F("EV_JOINED"));
    break;

  // This event is defined but not used in the code. No
  // point in wasting codespace on it.
  //
  // case EV_RFU1:
  //    Serial.println(F("EV_RFU1"));
  //    break;
  //
  case EV_JOIN_FAILED:
    Serial.println(F("EV_JOIN_FAILED"));
    break;
  case EV_REJOIN_FAILED:
    Serial.println(F("EV_REJOIN_FAILED"));
    break;
  case EV_TXCOMPLETE:
    Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
    // resetValues();
    if (LMIC.txrxFlags & TXRX_ACK)
      Serial.println(F("Received ack"));
    if (LMIC.dataLen)
    {
      Serial.println(F("Received "));
      Serial.println(LMIC.dataLen);
      Serial.println(F(" bytes of payload"));
    }
    // Schedule next transmission
    os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL),
                        do_send);
    break;
  case EV_LOST_TSYNC:
    Serial.println(F("EV_LOST_TSYNC"));
    break;
  case EV_RESET:
    Serial.println(F("EV_RESET"));
    break;
  case EV_RXCOMPLETE:
    // data received in ping slot
    Serial.println(F("EV_RXCOMPLETE"));
    break;
  case EV_LINK_DEAD:
    Serial.println(F("EV_LINK_DEAD"));
    break;
  case EV_LINK_ALIVE:
    Serial.println(F("EV_LINK_ALIVE"));
    break;
    //
    // This event is defined but not used in the code. No
    // point in wasting codespace on it.
    //
    // case EV_SCAN_FOUND:
    //  Serial.println(F("EV_SCAN_FOUND"));
    //  break;

  case EV_TXSTART:
    Serial.println(F("EV_TXSTART"));
    break;
  case EV_TXCANCELED:
    Serial.println(F("EV_TXCANCELED"));
    break;
  case EV_RXSTART:
    // do not print anything -- it wrecks timing
    break;
  case EV_JOIN_TXCOMPLETE:
    Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
    break;
  default:
    Serial.print(F("Unknown event: "));
    Serial.println((unsigned)ev);
    break;
  }
}

//
// PAYLOAD CONFIG
// Bytes in Payload depend on (your) measurements -> change if
// needed
//
void generate_payload()
{
  // See https://www.aeq-web.com/so-funktioniert-die-lora-payload-encoder-decoder-ttn/
  // for more information on payload encoding.
  //
  // we cant transfer floats that is why we multiply with 100
  // to remove the decimal values from the float
  // since we also cant pass negative values we add 5000 to the measurement
  // we need to remove these 5000 again on the decoder side
  // function decodeUplink(input){
  //   const tmp = (input.bytes[0] << 8 | input.bytes[1]);
  //   return {
  //     data:{
  //         measurements:[(tmp-5000)/100)]
  //       },
  //       warnings:[],
  //       errors:[]
  //     }
  // }
  int tmp = ((int)(measurement * 100)) + 5000;
  Serial.println("Measurement: " + String(measurement));
  Serial.println("\n- - - - - - - - - - - - - - - -\n");

  tx_payload[0] = tmp >> 8;
  tx_payload[1] = tmp;
}

//
// TTN UPLINK
// Values which will be uploaded depend on (your) measurements
// --> change
//
void do_send(osjob_t *j)
{
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND)
  {
    Serial.println(F("OP_TXRXPEND, not sending"));
  }
  else
  {

    Serial.println("\n----------------------------");
    Serial.println("Sending playload now");
    Serial.println("- - - - - - - - - - - - - - - -\n");

    generate_payload();

    Serial.print("Payload: ");
    int x = 0;
    while (x < sizeof(tx_payload))
    {
      printHex2(tx_payload[x]);
      Serial.print(" ");
      x++;
    }
    Serial.println();

    LMIC_setTxData2(1, tx_payload, sizeof(tx_payload), 0);
    Serial.println(F("Packet queued"));
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

void setup()
{
  // TTN
  Serial.begin(115200);
  SPI.begin(5, 19, 27, 18); //(MasterIN, SlaveOut,etc.)
  while (!Serial)
    ;         // wait for Serial to be initialized
  delay(100); // per sample code on RF_95 test
  Serial.println(F("Starting"));

  // LMIC init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  // Set static session parameters. Instead of dynamically establishing a
  // session by joining the network, precomputed session parameters are be
  // provided.
#ifdef PROGMEM
  // On AVR, these values are stored in flash and only copied to RAM
  // once. Copy them to a temporary buffer here, LMIC_setSession will
  // copy them into a buffer of its own again.
  uint8_t appskey[sizeof(APPSKEY)];
  uint8_t nwkskey[sizeof(NWKSKEY)];
  memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
  memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
  LMIC_setSession(0x1, DEVADDR, nwkskey, appskey);
#else
  // If not running an AVR with PROGMEM, just use the arrays directly
  LMIC_setSession(0x1, DEVADDR, NWKSKEY, APPSKEY);
#endif

#if defined(CFG_eu868)
  // Set up the channels used by the Things Network, which corresponds
  // to the defaults of most gateways. Without this, only three base
  // channels from the LoRaWAN specification are used, which certainly
  // works, so it is good for debugging, but can overload those
  // frequencies, so be sure to configure the full frequency range of
  // your network here (unless your network autoconfigures them).
  // Setting up channels should happen after LMIC_setSession, as that
  // configures the minimal channel set.
  // NA-US channels 0-71 are configured automatically

  LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),
                    BAND_CENTI); // g-band
  LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B),
                    BAND_CENTI); // g-band
  LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),
                    BAND_CENTI); // g-band
  LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),
                    BAND_CENTI); // g-band
  LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),
                    BAND_CENTI); // g-band
  LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),
                    BAND_CENTI); // g-band
  LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),
                    BAND_CENTI); // g-band
  LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),
                    BAND_CENTI); // g-band
  LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK, DR_FSK),
                    BAND_MILLI); // g2-band

  // LMIC_disableChannel(0); //Send only at channel 0
  LMIC_disableChannel(1);
  LMIC_disableChannel(2);
  LMIC_disableChannel(3);
  LMIC_disableChannel(4);
  LMIC_disableChannel(5);
  LMIC_disableChannel(6);
  LMIC_disableChannel(7);
  LMIC_disableChannel(8);

  // TTN defines an additional channel at 869.525Mhz using SF9 for class B
  // devices' ping slots. LMIC does not have an easy way to define set this
  // frequency and support for class B is spotty and untested, so this
  // frequency is not configured here.
#elif defined(CFG_us915)
  // NA-US channels 0-71 are configured automatically
  // but only one group of 8 should (a subband) should be active
  // TTN recommends the second sub band, 1 in a zero based count.
  // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
  LMIC_selectSubBand(1);
#endif

  // Disable link check validation
  LMIC_setLinkCheckMode(0);

  // TTN uses SF9 for its RX2 window.
  LMIC.dn2Dr = DR_SF9;

  // Set data rate and transmit power for uplink
  // (note: txpow seems to be ignored by the library)
  LMIC_setDrTxpow(DR_SF7, 14);
  // TODO: More the code below to loop when you deploy

  // Adafruit temp sensor setup

  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example, also can be left in blank for default address use
  // Also there is a table with all addres possible for this sensor, you can connect multiple sensors
  // to the same i2c bus, just configure each sensor with a different address and define multiple objects for that
  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F
  if (!tempsensor.begin(0x18))
  {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1)
      ;
  }

  Serial.println("Found MCP9808!");

  tempsensor.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
  // Start job
  do_send(&sendjob);
}

void loop()
{
  os_runloop_once();

  if (millis() >= measuringIteration * measuringPeriod)
  {
    sample_data();
    measuringIteration += 1;
  }
}
