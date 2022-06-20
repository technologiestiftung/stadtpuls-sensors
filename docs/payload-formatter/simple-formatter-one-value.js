// This is a simple example for a payload formatter function.
// In this example we are decoding values we received from the MCP9808 temperatur sensor.
// As used in the programm ./lora-temperature
// Since we can't have negative nor floating point values encoded as bytes we multiply the values
// by 100 and and add 5000 so we can be sure to be in a positive range.
// Therefore we need to subtract 5000 and divide by 100 again when decoding.
// See https://github.com/technologiestiftung/stadtpuls-sensors/blob/98a76cab87f3c43f5fcb66543262fa0e91ed4940/temperature-lora/src/main.cpp#L322
// Language: javascript
//
//
function decodeUplink(input) {
  const tmp = (input.bytes[0] << 8) | input.bytes[1];
  return {
    data: {
      measurements: [(tmp - 5000) / 100],
    },
    warnings: [],
    errors: [],
  };
}
