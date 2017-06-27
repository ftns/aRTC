//$Id: i2c_rtc.h,v 1.4 2017/06/25 13:45:31 akihiro Exp akihiro $
// i2c_rtc.h

#ifndef __I2C_RTC_H__
#define __I2C_RTC_H__

#include "aRTC.h"
#include <Wire.h>
#include <initializer_list>

#define RTC_I2CERR 0x80 // I2C related error
#define RTC_I2CW   0x81 // error when writing
#define RTC_I2CR   0x82 // error when reading

#define RTC_I2C_DEBUG0 0x88
#define RTC_I2C_DEBUG1 0x89
#define RTC_I2C_DEBUG2 0x8A
#define RTC_I2C_DEBUG3 0x8B
#define RTC_I2C_DEBUG4 0x8C


class i2c_rtc : public aRTC {
public:
  i2c_rtc(){};
  void setWire(TwoWire w){
    _wire = w;
  };
  // CAUTION: if you want change I2C I/F from Wire,
  // setWire() must be called before aRTC::startRTC().
  // Because almost all subclasses use _wire in startRTC();

protected:
  TwoWire _wire = Wire;
  uint8_t _addr;

  uint32_t _timeout = 10000; // default 10ms = 10,000 micro sec
  uint32_t _t_buf = 100; // default: 100 micro sec

  bool _waitReadable(unsigned long start);
  
  bool _writeBytes(bool stopBit, std::initializer_list<const uint8_t> data);
  bool _readBytes(bool stopBit, std::initializer_list<uint8_t *> vars);
};

#endif // ifndef __I2C_RTC_H__

// end of i2c_rtc.h
