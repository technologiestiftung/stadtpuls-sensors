# Stadtpuls Infrared Sensors

Basic Setup for running the VL53L1X infrared sensor on an Arduino Board using PlatformIO.

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
- Arduino Uno 
- two [Adafruit VL53L1X sensors](https://thepihut.com/products/adafruit-vl53l1x-time-of-flight-distance-sensor-30-to-4000mm-stemma-qt-qwiic)
- bread board
- cables

Power supply is given by the micro USB cable and an attached power bank.

### Specs of the ultrasonic sensor
| What         | VL53L1X Specs |
| ------------ | ------------- |
| min range    | 2cm           |
| max range    | 4 meters      |
| Power Supply | 5V DC         |

## Pin mapping to Heltec board
|     | IN VL53L1X | OUT VL53L1X |
| --- | ---------- | ----------- |
| VCC | 5v         | 5v          |
| SDA | SDA        | SDA         |
| SCL | SCL        | SCL         |
| GND | GND        | GND         |


This approach strongly follows [this beautiful GitHub Repo](https://github.com/pololu/vl53l1x-arduino/blob/master/examples/ContinuousMultipleSensors/ContinuousMultipleSensors.ino
).

Follow [this YouTube video](https://www.youtube.com/watch?v=Sc_iVfeocvg) to get a first idea of the infrared sensor.