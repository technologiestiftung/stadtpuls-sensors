![](https://img.shields.io/badge/Built%20with%20%E2%9D%A4%EF%B8%8F-at%20Technologiestiftung%20Berlin-blue)

# Temperature over HTTP(s)

As seen on [re:publica 2022 in the maker-space](https://re-publica.com/en/node/359)

<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->

[![All Contributors](https://img.shields.io/badge/all_contributors-3-orange.svg?style=flat-square)](#contributors-)

<!-- ALL-CONTRIBUTORS-BADGE:END -->

<!--

How to:

- You don't need to use every section. Only the ones that apply to your project.
- Adjust the files on .github/ISSUE_TEMPLATE/* how you need them
- Adjust the file on .github/CODEOWNERS to match your team
- If you use staging and main branches use this template for .github/renovate.json


```json
{
   "$schema": "https://docs.renovatebot.com/renovate-schema.json",
  "extends": [
    "@inpyjamas"
  ],
    "baseBranches": [
    "staging"
  ]
}
```

Bonus:

Use all-contributors

npx all-contributors-cli check
npx all-contributors-cli add ff6347 doc

You can use it on GitHub just by commenting on PRs and issues:

```
@all-contributors please add @ff6347 for infrastructure, tests and code
```
Read more here https://allcontributors.org/


Get fancy shields at https://shields.io
 -->

## Prerequisites

- PlatformIO - https://platformio.org/

## Installation

```bash
pip install platformio
# or
brew install platformio
```

## Usage & Development

```bash
# install all needed libraries
pio pkg install
# build the binary locally
pio run
# upload all the files in the ./data folder to the SPIFFS
pio run --target uploadfs
# upload the binary to the board
pio run --target upload
# see its output in the serial monitor
pio device monitor -b 115200
```

## Wiring

```txt
//                                             HELTEC WIFI Kit 32
//
//                                      +-----------------------------+
//      MCP9808                         |                             |
//                                      |                             |
// +---------------+                    |                             |
// |               |                    |                             |
// |         VCC  -+- ---------------- -+-   5V                      -+
// |               |                    |                             |
// |         GND  -+- ---------------- -+-   GND                     -+
// |               |                    |                             |
// |         SCL  -+- ---------------- -+-   SCL (22)                -+
// |               |                    |                             |
// |         SDA  -+- ---------------- -+-   SDA (23)                -+
// |               |                    |                             |
// |          A0  -+-                   |       to forget WiFi   33  -+
// |               |                    |       credentials connect   |
// |          A1  -+-                   |       pin 33 to VCC         |
// |               |                    |                             |
// |          A2  -+-                   |                             |
// |               |                    |                             |
// |       ALERT  -+-                   |                             |
// |               |                    |                             |
// +---------------+                    |                             |
//                                      |                             |
//                                      |                             |
//                                      |                             |
//                                      |                             |
//                                      |                             |
//                                      |                             |
//                                      +-----------------------------+
```

![](./docs/wiring-temp-step-by-step_bb-8.png)

## Contributors

Thanks goes to these wonderful people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://fabianmoronzirfas.me/"><img src="https://avatars.githubusercontent.com/u/315106?v=4?s=64" width="64px;" alt=""/><br /><sub><b>Fabian Morón Zirfas</b></sub></a><br /><a href="https://github.com/technologiestiftung/stadtpuls-sensors/commits?author=ff6347" title="Code">💻</a> <a href="https://github.com/technologiestiftung/stadtpuls-sensors/commits?author=ff6347" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/julizet"><img src="https://avatars.githubusercontent.com/u/52455010?v=4?s=64" width="64px;" alt=""/><br /><sub><b>Julia Zet</b></sub></a><br /><a href="https://github.com/technologiestiftung/stadtpuls-sensors/commits?author=julizet" title="Code">💻</a> <a href="https://github.com/technologiestiftung/stadtpuls-sensors/commits?author=julizet" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/dnsos"><img src="https://avatars.githubusercontent.com/u/15640196?v=4?s=64" width="64px;" alt=""/><br /><sub><b>Dennis Ostendorf</b></sub></a><br /><a href="https://github.com/technologiestiftung/stadtpuls-sensors/commits?author=dnsos" title="Code">💻</a> <a href="https://github.com/technologiestiftung/stadtpuls-sensors/commits?author=dnsos" title="Documentation">📖</a></td>
    <td align="center"><a href="http://www.awsm.de/"><img src="https://avatars.githubusercontent.com/u/434355?v=4?s=64" width="64px;" alt=""/><br /><sub><b>Ingo Hinterding</b></sub></a><br /><a href="https://github.com/technologiestiftung/stadtpuls-sensors/commits?author=Esshahn" title="Code">💻</a> <a href="https://github.com/technologiestiftung/stadtpuls-sensors/commits?author=Esshahn" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/vogelino"><img src="https://avatars.githubusercontent.com/u/2759340?v=4?s=64" width="64px;" alt=""/><br /><sub><b>Lucas Vogel</b></sub></a><br /><a href="https://github.com/technologiestiftung/stadtpuls-sensors/commits?author=vogelino" title="Code">💻</a> <a href="https://github.com/technologiestiftung/stadtpuls-sensors/commits?author=vogelino" title="Documentation">📖</a></td>
  </tr>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!

## Content Licensing

Texts and content available as [CC BY](https://creativecommons.org/licenses/by/3.0/de/).

## Credits

<table>
  <tr>
    <td>
      Made by <a src="https://citylab-berlin.org/de/start/">
        <br />
        <br />
        <img width="200" src="https://citylab-berlin.org/wp-content/uploads/2021/05/citylab-logo.svg" />
      </a>
    </td>
    <td>
      A project by <a src="https://www.technologiestiftung-berlin.de/">
        <br />
        <br />
        <img width="150" src="https://citylab-berlin.org/wp-content/uploads/2021/05/tsb.svg" />
      </a>
    </td>
    <td>
      Supported by <a src="https://www.berlin.de/rbmskzl/">
        <br />
        <br />
        <img width="80" src="https://citylab-berlin.org/wp-content/uploads/2021/12/B_RBmin_Skzl_Logo_DE_V_PT_RGB-300x200.png" />
      </a>
    </td>
  </tr>
</table>

## Related Links

- Rui Santos - esp32 tutorials - https://RandomNerdTutorials.com/
- Adafruit_MCP9808_Library - examples (temp sensor) - https://github.com/adafruit/Adafruit_MCP9808_Library
- Adafruit_SSD1306_Library - examples (oled display) - https://github.com/adafruit/Adafruit_SSD1306
- Adafruit_GFX_Library - examples (oled display) - https://github.com/adafruit/Adafruit-GFX-Library
- espressif arduino-esp32 examples - https://github.com/espressif/arduino-esp32/
- Heltec_ESP32 - examples (board) - https://github.com/HelTecAutomation/Heltec_ESP32/
