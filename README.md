# QDeconz2MQTT
QDeconz2MQTT publishes deconz events to a MQTT broker.
It is published under the GPL3.0+ license.

Currently only sensor events are supported and the state part of the events is forwarded to MQTT topic:
  deconz/_type_/_uniqueid_

The application expects a config file in INI format, as described in section [Configuration](#Configuration).
If no `--config` Option is given, the INI file is expected in the same directory as the binary,
with the name _qdeconz2mqtt.ini_.

## Dependencies
| Dependency  | Link                                                             |
| ----------- | ---------------------------------------------------------------- |
| deCONZ:     | https://www.dresden-elektronik.com/wireless/software/deconz.html |
| MQTT:       | https://mqtt.org/                                                |
| QtMQTT:     | https://doc.qt.io/qt-5/qtmqtt-index.html                         |
| QWebSocket: | https://doc.qt.io/qt-5/qwebsocket.html                           |

### Install QtMQTT on raspbian

```Bash
sudo apt-get install qtbase5-private-dev qt5-qtwebsockets-dev
git clone https://github.com/qt/qtmqtt.git --branch 5.15.2
cd qtmqtt
qmake-qt5 qtmqtt.pro
make
sudo make install
```

## Configuration
The configuration is given in an INI file.

The deconz websocket is given as websocket URL by the _url_ attribute.

The MQTT connection parameters are given by the _hostname_, _port_, _username_, _password_, _version_ and _usetls_ attributes.
If no authentification is needed, _username_ and _password_ need to be empty.
The _version_ attribute excepts three values: 3 for MQTT 3.1, 4 for MQTT 3.1.1 and 5 for MQTT 5.0.
If the MQTT connection is TLS encrypted the _usetls_ attribute should be set to true, false otherwise.
If the _retain_ attribute is True, the retain bit will be set during publishing the data.

```INI
[mqtt]
hostname=example.com
port=8883
username=USER
password=PASSWORT
version=3
usetls=true
retain=false

[websocket]
url=ws://example.com:443
```

