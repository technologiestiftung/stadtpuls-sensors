# Stadtpuls Ultra Sonic Sensors

Basic Setup for running electrec mics with Heltec Wifi Kit 32 v2 using Platformio.

## Setup

Install Platformio Core CLI (or the vscode plugin) https://docs.platformio.org/en/latest/

In global environment install platformIO CLI
```bash
brew install platformio
```

```bash
cd path/to/this/folder/
# https://docs.platformio.org/en/latest/core/userguide/index.html
pio lib install
pio run
pio run --target upload
pio device monitor --baud 115200 --eol LF
```
