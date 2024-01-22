# Minka Aire Fan MQTT

## Overview
ESP8266 project enabling MQTT control for a Minka Aire fan with a wireless receiver. Wireless communication is performed with a CC1101 wireless transceiver operating at 303 MHz.
This is based on Ben Owen's work for the [Hampton Bay Fan MQTT](https://github.com/owenb321/hampton-bay-fan-mqtt). Following his work taught me how to make mine work.

Notable differences bewtween the Hampton Bay and Minka Aire is that it uses 5 dip switches instead of 4. The rest of the logic works very similarly.
Fan control is not limited to a single dip switch setting, so up to 25 fans can be controlled with one ESP8266.

The code transmitted by the remote is made up of three sections that total 12 bits. The first five bits of the code are the inverse of the identification dip positions. So if the dip switches are; on, off, off, off, off. The code would start with 01111. The next is a single bit to represent the position of the dimmer on/off switch. This dip switch has ‘ON’ on the bottom rather than the top like the identification switches. The ‘ON’ position corresponds with a 1 being sent by the remote. 

The rest of the code is made up of these commands from the buttons:

Light:
Up (On)	   110110
Down (Off)	111110

Fan:
Off		      111101
1		        110111
2		        110101
3		        101111
4		        100111
5		        011101
6	 	       011111
Reverse	   111011

Based on the image bellow, all messages sent from the remote in this picture would start with 011111. The remaining six bits would correspond a particular button.
![alt text](https://github.com/oryweaver/minka-aire-fan-mqtt/blob/master/images/RC400_remote.jpg)

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
  * `home/minka-aire/<fan_id>/on/state`
  * `home/minka-aire/<fan_id>/on/set`
* Fan speed (payload `one`, `two`, `three`, `four`, `five`, `six`)
  * `home/minka-aire/<fan_id>/speed/state`
  * `home/minka-aire/<fan_id>/speed/set`
* Fan direction (payload `forward` or `reverse`)
  * `home/minka-aire/<fan_id>/dir`
  * `home/minka-aire/<fan_id>/dirState`
* Light on/off (payload `ON` or `OFF`)
  * `home/minka-aire/<fan_id>/light/state`
  * `home/minka-aire/<fan_id>/light/set`

`fan_id` is a 5-digit binary number determined by the dip switch settings on the transmitter/receiver where up = 1 and down = 0. For example, the dip setting:

|1|2|3|4|5|
|-|-|-|-|-|
|↑|↓|↓|↓|↓|

...corresponds to a fan ID of `10000`

### Home Assistant
To use this in Home Assistant as an MQTT Fan and MQTT Light, I'm using this config
```yaml
input_boolean:
  rf_light_state:
    name: "RF Light State"
    initial: off  # or on, depending on the initial state of your light when HA starts

mqtt:
  fan:
  - name: "Bedroom Fan"
    unique_id: private
    state_topic: "home/minka-aire/<fan_id>/state"
    command_topic: "home/minka-aire/<fan_id>/command"
    preset_modes:
      - 'off'
      - 'one'
      - 'two'
      - 'three'
      - 'four'
      - 'five'
      - 'six'
    preset_mode_command_topic: "home/minka-aire/<fan_id>/speed/set"
    preset_mode_state_topic: "home/minka-aire/<fan_id>/speed/state"
    direction_command_topic: "home/minka-aire/<fan_id>/dir"
    direction_state_topic: "home/minka-aire/<fan_id>/dirState"
    availability_topic: "home/minka-aire/availability"

  light:
  - name: "Bedroom Fan Light"
    unique_id: private
    state_topic: "home/minka-aire/<fan_id>/light/state"
    command_topic: "home/minka-aire/<fan_id>/light/command"
    availability_topic: "home/minka-aire/availability"
    state_value_template: "{{ states('input_boolean.rf_light_state') }}"
```
