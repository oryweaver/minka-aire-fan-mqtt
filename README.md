# Minka Aire Fan MQTT

## Overview
ESP8266 project enabling MQTT control for a Minka Aire fan with a wireless receiver. Wireless communication is performed with a CC1101 wireless transceiver operating at 303 MHz.
This is based on Ben Owen's work for the [Hampton Bay Fan MQTT](https://github.com/owenb321/hampton-bay-fan-mqtt). Following his work taught me how to make mine work.

Notable differences bewtween the Hampton Bay and Minka Aire is that it uses 5 dip switches instead of 4. The rest of the logic works verry similla
Fan control is not limited to a single dip switch setting, so up to 25 fans can be controlled with one ESP8266.

## Dependencies
This project uses the following libraries that are available through the Arduino IDE
* [SmartRC-CC1101-Driver-Lib](https://github.com/LSatan/SmartRC-CC1101-Driver-Lib) by LSatan
* [rc-switch](https://github.com/sui77/rc-switch) by sui77
* [PubSubClient](https://pubsubclient.knolleary.net/) by Nick O'Leary

## Hardware
* ESP8266 development board (Tested with a NodeMCU v3)
* CC1101 wireless transceiver
  * Wiring info can be found in the [SmartRC-CC1101-Driver-Lib readme](https://github.com/LSatan/SmartRC-CC1101-Driver-Lib#wiring)

## Setup
### Configuration
Change the `WIFI_*` and `MQTT_*` definitions in the sketch to match your network settings before uploading to the ESP.
### MQTT
By default, the state/command topics will be
* Fan on/off (payload `ON` or `OFF`)
  * `home/hamptonbay/<fan_id>/on/state`
  * `home/hamptonbay/<fan_id>/on/set`
* Fan speed (payload `low`, `medium`, or `high`)
  * `home/hamptonbay/<fan_id>/speed/state`
  * `home/hamptonbay/<fan_id>/speed/set`
* Light on/off (payload `ON` or `OFF`)
  * `home/hamptonbay/<fan_id>/light/state`
  * `home/hamptonbay/<fan_id>/light/set`

`fan_id` is a 4-digit binary number determined by the dip switch settings on the transmitter/receiver where up = 1 and down = 0. For example, the dip setting:

|1|2|3|4|
|-|-|-|-|
|↑|↓|↓|↓|

...corresponds to a fan ID of `1000`

### Home Assistant
To use this in Home Assistant as an MQTT Fan and MQTT Light, I'm using this config
```yaml
fan:
- platform: mqtt
  name: "Bedroom Fan"
  state_topic: "home/hamptonbay/1000/on/state"
  command_topic: "home/hamptonbay/1000/on/set"
  speed_state_topic: "home/hamptonbay/1000/speed/state"
  speed_command_topic: "home/hamptonbay/1000/speed/set"
  speeds:
    - low
    - medium
    - high

light:
- platform: mqtt
  name: "Bedroom Fan Light"
  state_topic: "home/hamptonbay/1000/light/state"
  command_topic: "home/hamptonbay/1000/light/set"
```
