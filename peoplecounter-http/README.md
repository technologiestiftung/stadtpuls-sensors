# Stadtpuls Audio Sensors

Basic Setup for running electrec mics with Heltec Wifi Kit 32 v2 using PlatformIO.

## Software Setup

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

## Hardware Setup EPS32 Peoplecounter

The people counter uses the following hardware components:
- Heltec v2
- two HC-SR04 Ultrasoic sensor
- button
- bread board
- cables, 220ohm ristors

Power supply is given by the micro USB cable and an attached power bank.

### Specs of the sensor
| What              | HC-SR04 Specs |
| ----------------- | ------------- |
| min range         | 2cm           |
| max range         | 4 meters      |
| resolution        | 3 cm          |
| Power Supply      | 5V DC         |
| Working Frequency | 40kHz         |

## Pin mapping to Heltec board
|      | IN HC-SR04 | OUT HC-SR04 |
| ---- | ---------- | ----------- |
| VCC  | 5v         | 5v          |
| Trig | GPIO 12    | GPIO 32     |
| Echo | GPIO 13    | PIO 33      |
| GND  | GND        | GND         |


This approach strongly follows this beautiful tutorial: https://create.arduino.cc/projecthub/ryanchan/room-occupancy-counter-3b3ffa?ref=part&ref_id=10308&offset=2
