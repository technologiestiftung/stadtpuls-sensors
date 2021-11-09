/*******************************************************************************
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Copyright (c) 2018 Terry Moore, MCCI
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
 *******************************************************************************/

#include <Arduino.h>
// +++ LoRaWan TTN library
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

// +++ OLED display library
#include <U8x8lib.h>
#include <U8g2lib.h>

void do_send(osjob_t* j);
//--------------------------------------------------------------------------------------------
//                STATIC VARS AND DEFINES
//--------------------------------------------------------------------------------------------

//------------------------------------Sensor Variables-----------------------------------------
// do change these values if you don't need all of them
float currentDecibels = NULL;
float coeff0 = 2785.7384386736867;//83.2073;
float coeff1 = 58.66759823916591;//11.003;
float prevDecibels = NULL;
int SOUND_SENSOR_PIN = 36;
const char* deviceNo = "vog-02"; // type your device ID
unsigned int sample;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)

int adc;
int dB, PdB; //the variable that will hold the value read from the microphone each time

//------------------------------------Stadtpuls Logo--------------------------------------------
// pls do not change
#define logo_width 118
#define logo_height 22
static unsigned char  logo_bits[] = {
  0xD7, 0xBD, 0xEE, 0xEC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x3F, 0x7D, 0xAB, 0x6B, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x83, 0xD0, 0xB1, 0xF8, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0xE0, 0x71, 0x00,
  0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F,
  0x63, 0xB1, 0x71, 0xE8, 0xFF, 0x3F, 0xFF, 0xFF, 0xCF, 0xF3, 0xFF, 0xFF,
  0x9F, 0xFF, 0x3F, 0x55, 0xA6, 0xC9, 0xF5, 0xFF, 0x3F, 0xFF, 0xFF, 0xCF,
  0xF3, 0xFF, 0xFF, 0x9F, 0xFF, 0x3F, 0xB7, 0xEF, 0xBB, 0xF8, 0xFF, 0x3F,
  0xFF, 0xFF, 0xCF, 0xF3, 0xFF, 0xFF, 0x9F, 0xFF, 0x3F, 0xE1, 0x79, 0xEE,
  0xED, 0x3F, 0x0E, 0x7C, 0xFE, 0xC9, 0xC0, 0xC9, 0xF7, 0x9D, 0xC7, 0x3F,
  0x51, 0xCD, 0x9E, 0xFD, 0x8F, 0x18, 0x1C, 0x78, 0xC0, 0xC1, 0x11, 0xE7,
  0x99, 0x91, 0x3F, 0xF7, 0x7B, 0xF7, 0xF0, 0xCF, 0x3D, 0xDF, 0x3B, 0xCF,
  0xF3, 0x79, 0xE6, 0x99, 0xB9, 0x3F, 0x82, 0xE8, 0x21, 0xE8, 0x8F, 0x3F,
  0xFF, 0x3B, 0xCF, 0xF3, 0x79, 0xE6, 0x99, 0xF1, 0x3F, 0xE3, 0x01, 0xF0,
  0xF8, 0x3F, 0x3C, 0x1F, 0x38, 0xCF, 0xF3, 0x79, 0xE6, 0x99, 0x87, 0x3F,
  0x20, 0xD1, 0xF9, 0xE0, 0xFF, 0x39, 0xCF, 0x3B, 0xCF, 0xF3, 0x79, 0xE6,
  0x99, 0x3F, 0x3F, 0xFF, 0x7B, 0x45, 0xFD, 0xCF, 0x39, 0xCF, 0x39, 0xCF,
  0xF3, 0x79, 0xE7, 0x98, 0x39, 0x3F, 0xDA, 0xD9, 0xF7, 0xF4, 0x0F, 0x78,
  0x1F, 0x70, 0xC0, 0xF7, 0x01, 0x0F, 0x98, 0x01, 0x3F, 0x5B, 0x73, 0x9E,
  0xFD, 0x7F, 0x7E, 0x7C, 0xFE, 0xC9, 0xC7, 0xC9, 0x9F, 0x99, 0xCF, 0x3F,
  0xB9, 0x7B, 0x7B, 0xEC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFF,
  0xFF, 0xFF, 0x3F, 0xA7, 0x52, 0xE1, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xF9, 0xFF, 0xFF, 0xFF, 0x3F, 0xE3, 0xF0, 0xA0, 0xF8, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFF, 0xFF, 0xFF, 0x3F, 0x62, 0xA3, 0xB1,
  0xE8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F,
  0xBE, 0x5D, 0x7F, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0x3F, 0xE3, 0xF5, 0xD2, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, };


//--------------------------------------------------------------------------------------------
//                DISPLAY SETUP --> do not change
//--------------------------------------------------------------------------------------------
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);

// do not change
void drawScreen() {
    if (prevDecibels != currentDecibels) {
      u8x8.clear();

      u8g2.begin();
      u8g2.drawXBM(5,0, logo_width, logo_height, logo_bits);
      u8g2.setFont(u8g2_font_6x12_te); // 7 pixel high
      u8g2.drawStr(5, 34, "DevID: " );
      u8g2.drawStr(64, 34, deviceNo);

      u8g2.drawStr(5, 46, "Decibels: " );
      if (currentDecibels != NULL) {
        u8g2.setCursor(64, 46);
        String decibelsDisplay = (String)currentDecibels + " db";
        u8g2.print(decibelsDisplay); //print float with one position
      }
      else {
        u8g2.drawStr(64, 46, "NULL" );
      }

      u8g2.sendBuffer(); // Inhalt des Speichers direkt auf das Display pushen
      prevDecibels = currentDecibels;
    }
}
//--------------------------------------------------------------------------------------------
//                TTN Credentials --> get them from your TTN application console
//--------------------------------------------------------------------------------------------
// LoRaWAN NwkSKey, network session key – should be in big-endian (aka msb).
static const PROGMEM u1_t NWKSKEY[16] = { 0x72, 0xB3, 0xF3, 0x84, 0x7D, 0x31, 0xC6, 0x05, 0xF6, 0xE0, 0x7D, 0x32, 0x3F, 0xE9, 0x05, 0x26 };

// LoRaWAN AppSKey, application session key – should also be in big-endian (aka msb).
static const u1_t PROGMEM APPSKEY[16] = { 0x1A, 0x08, 0xDD, 0x95, 0x4E, 0x16, 0xE1, 0xDB, 0x0C, 0xF3, 0x62, 0xC3, 0xAC, 0x5F, 0x52, 0x30 };

// LoRaWAN end-device address (DevAddr) – hould be in big-endian (aka msb) too
// See http://thethingsnetwork.org/wiki/AddressSpace
static const u4_t DEVADDR = 0x260B357B; // <-- Change this address for every node!

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in arduino-lmic/project_config/lmic_project_config.h,
// otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

uint8_t tx_payload[8];
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = 60 * 5;

//--------------------------------------------------------------------------------------------
//                PIN MAPPING FOR HELTEC ESP32 V2 --> do not change
//--------------------------------------------------------------------------------------------
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 14,                       // reset pin
    .dio = {26, 34, 35},
};

//--------------------------------------------------------------------------------------------
//                PAYLOAD HELPER FUNCTION --> do not change
//--------------------------------------------------------------------------------------------

void printHex2(unsigned v) {
  v &= 0xff;
  if (v < 16)
    Serial.print('0');
  Serial.print(v, HEX);
}

//--------------------------------------------------------------------------------------------
//                TTN DEBUGGING
//                Messages can be checked withing your serial monitor. Do not change!
//--------------------------------------------------------------------------------------------

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
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
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_RFU1:
        ||     Serial.println(F("EV_RFU1"));
        ||     break;
        */
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
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
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_SCAN_FOUND:
        ||    Serial.println(F("EV_SCAN_FOUND"));
        ||    break;
        */
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            break;
        case EV_TXCANCELED:
            Serial.println(F("EV_TXCANCELED"));
            break;
        case EV_RXSTART:
            /* do not print anything -- it wrecks timing */
            break;
        case EV_JOIN_TXCOMPLETE:
            Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
            break;
        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            break;
    }
}


//--------------------------------------------------------------------------------------------
//               PAYLOAD CONFIG
//               Bytes in Payload depend on (your) measurements -> change if needed
//--------------------------------------------------------------------------------------------

void generate_payload() {
  int decibelsTimesHundred = currentDecibels * 100;

  tx_payload[0] = (int)decibelsTimesHundred >> 8;
  tx_payload[1] = (int)decibelsTimesHundred;
}

//--------------------------------------------------------------------------------------------
//                TTN UPLINK
//                Values which will be uploaded depend on (your) measurements --> change
//--------------------------------------------------------------------------------------------

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    }
    else {

    generate_payload();

    Serial.print("Payload: ");
    int x = 0;
    while (x < sizeof(tx_payload)) {
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


void updateValues() {
    PdB = dB; //Store the previous of dB here
    adc= analogRead(SOUND_SENSOR_PIN); //Read the ADC value from amplifer
//Serial.println (adc);//Print ADC for initial calculation
  currentDecibels = (adc+coeff0) / coeff1; //Convert ADC value to dB using Regression values
  // unsigned long startMillis = millis();                   // Start of sample window
  // float peakToPeak = 0;                                   // peak-to-peak level

  // unsigned int signalMax = 0;                             //minimum value
  // unsigned int signalMin = 4095;                          //maximum value
  //                                                         // collect data for 50 mS
  // while (millis() - startMillis < sampleWindow) {
  //   sample = analogRead(SOUND_SENSOR_PIN);                //get reading from microphone

  //   if (sample < 4095) {                                  // toss out spurious readings
  //        if (sample > signalMax) {
  //           signalMax = sample;                           // save just the max levels
  //        }
  //        else if (sample < signalMin) {
  //           signalMin = sample;                           // save just the min levels
  //        }
  //     }
  // }

  // peakToPeak = signalMax - signalMin;                     // max - min = peak-peak amplitude
  // currentDecibels = map(peakToPeak, 20, 900, 42, 90);

  Serial.println(currentDecibels);

  String decibelsDisplay = "Decibels: " + (String)currentDecibels + " db";
}

void setup() {
//------------------------------------Display----------------------------------------------
    u8x8.begin();
    u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
//------------------------------------TTN--------------------------------------------------
    Serial.begin(115200);
    SPI.begin(5,19,27,18); //(MasterIN, SlaveOut,etc.)
    while (!Serial); // wait for Serial to be initialized
    delay(100);     // per sample code on RF_95 test
    Serial.println(F("Starting"));

     // LMIC init
     os_init();
     // Reset the MAC state. Session and pending data transfers will be discarded.
     LMIC_reset();
// Set static session parameters. Instead of dynamically establishing a session
   // by joining the network, precomputed session parameters are be provided.
 #ifdef PROGMEM
   // On AVR, these values are stored in flash and only copied to RAM
   // once. Copy them to a temporary buffer here, LMIC_setSession will
   // copy them into a buffer of its own again.
   uint8_t appskey[sizeof(APPSKEY)];
   uint8_t nwkskey[sizeof(NWKSKEY)];
   memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
   memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
   LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
 #else
   // If not running an AVR with PROGMEM, just use the arrays directly
   LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
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

   LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
   LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band

   //LMIC_disableChannel(0); //Send only at channel 0
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
   // <nowiki>https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json</nowiki>
   LMIC_selectSubBand(1);
 #endif

   // Disable link check validation
   LMIC_setLinkCheckMode(0);

   // TTN uses SF9 for its RX2 window.
   LMIC.dn2Dr = DR_SF9;

   // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
   LMIC_setDrTxpow(DR_SF7, 14);

   // Start job
   do_send(&sendjob);
}

void loop() {
    updateValues();
    // os_runloop_once(); // TTN
   drawScreen();
}