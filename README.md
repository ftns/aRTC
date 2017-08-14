# aRTC
aRTC is an abstruct RTC Class and library classes of RTCs for Arduino.

## Features
* All RTCs are subclass of aRTC, so you don't bother with differences between RTCs. (except specific feature dependes on each RTC.)
* Almost all errors are handled. So you can improve your code more reliable. Also, you can ignore errors for your convenience. (However, I STRONGLY recommend check errors.)
* You can handle NTP easilly with aRTC::SetEpoch().

## Supported RTCs:
Currently, all supported RTCs are connected to Arduino with I2C(wire) interface.

  * EPSON: RX8025, RTC8564
  * MAXIM: DS1307, DS3231
  * NXP:   PCF8523, PCF2129 

## Test environment
* Development environment
  * Arduino 1.8 or later
* Arduino model
  * ESP8266 (ESP-WROOM-02)
  
## Change Log
* 8/13/2017 - ver.1.0RC3
  * Release Candidate 3
  * Add ds3231 and i2cDevice classes.
  * Add some description on README.md
* 6/25/2017
  * initial version
