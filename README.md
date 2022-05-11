![](https://img.shields.io/badge/Built%20with%20%E2%9D%A4%EF%B8%8F-at%20Technologiestiftung%20Berlin-blue)

# Stadtpuls Sensors

Basic Setup for running sensors for stadtpuls.com with Platformio.

## Setup

Install Platformio Core CLI (or the vscode plugin) https://docs.platformio.org/en/latest/

```bash
cd path/to/this/folder/
# https://docs.platformio.org/en/latest/core/userguide/index.html
pio init --board <YOUR BOARD HERE>
```

## Environment Variables

For some sensors we use the https request method to post measurements to stadtpuls. To do so we need to adjust the environments variables within the `env.sample.h`

1. copy `env.sample.h.` to your `src` folder and rename to `env.h`
2. include `src` folder to library paths in platformIO
3. replace sample values with the values of your sensor in stadtpuls
4. for the certificate:
   1. open the URL of your server --> api.stadtpuls.com
   2. click on the lock icon to the left of your browser's search field
   3. show details about your certificate
   4. you might be shown multiple certifcates, take the last one (the one to the most right) and download it
5. save your `env.h`

## Contributors ✨

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

## Credits

<table>
  <tr>
    <td>
      <a src="https://citylab-berlin.org/de/start/">
        <br />
        <br />
        <img width="200" src="https://citylab-berlin.org/wp-content/uploads/2021/05/citylab-logo.svg" />
      </a>
    </td>
    <td>
      A project by: <a src="https://www.technologiestiftung-berlin.de/">
        <br />
        <br />
        <img width="150" src="https://citylab-berlin.org/wp-content/uploads/2021/05/tsb.svg" />
      </a>
    </td>
    <td>
      Supported by: <a src="https://www.berlin.de/rbmskzl/">
        <br />
        <br />
        <img width="80" src="https://citylab-berlin.org/wp-content/uploads/2021/12/B_RBmin_Skzl_Logo_DE_V_PT_RGB-300x200.png" />
      </a>
    </td>
  </tr>
</table>
