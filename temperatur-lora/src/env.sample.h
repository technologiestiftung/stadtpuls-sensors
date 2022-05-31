// rename this file to env.h and fill in the values
// const char *deviceNo = "xxxxx"; // type your device ID

//--------------------------------------------------------------------------------------------
//                TTN Credentials --> get them from your TTN application console
//--------------------------------------------------------------------------------------------
// LoRaWAN NwkSKey, network session key – should be in big-endian (aka msb).
static const PROGMEM u1_t NWKSKEY[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x00};

// LoRaWAN AppSKey, application session key – should also be in big-endian (aka
// msb).
static const u1_t PROGMEM APPSKEY[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                         0x00, 0x00, 0x00, 0x00};
// LoRaWAN end-device address (DevAddr) – should be in big-endian (aka msb) too
// See https://web.archive.org/web/20170728112659/https://www.thethingsnetwork.org/wiki/LoRaWAN/Address-Space
static const u4_t DEVADDR =
    0x12345678; // <-- Change this address for every node!