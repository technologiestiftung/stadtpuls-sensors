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
