//$Id: i2c_rtc.cpp,v 1.3 2017/06/25 13:45:31 akihiro Exp akihiro $
// i2c_rtc.cpp

#include "i2c_rtc.h"

bool i2c_rtc::_waitReadable(unsigned long start){
  while (! _wire.available()){
    if (diffULong(micros(), start)>_timeout) {
      _rtc_errno = RTC_I2CR;
      return false;
    };
    delayMicroseconds(10);
  }
  return true;
}


bool i2c_rtc::_writeBytes(bool stopBit, std::initializer_list<const uint8_t> data){
  int err;
  unsigned long t;

  t = micros();

  // loop until timeout(default 10ms=10000micro sec)
  while (diffULong(micros(), t) < _timeout) {
    delayMicroseconds(_t_buf);

    _wire.beginTransmission(_addr);

    for (auto b = data.begin(); b != data.end(); ++b)
      _wire.write(*b);
    
    err = _wire.endTransmission(stopBit);

    if (err == 0) {
      return true;
    } else if (err != 2) {
      _rtc_errno = RTC_I2CW;
      return false;
    }

    // if err==2, rtc is busy.
    delayMicroseconds(5000); // wait 5msec
  } // while

  _rtc_errno = RTC_I2CW;
  return false;
}


bool i2c_rtc::_readBytes(bool stopBit, std::initializer_list<uint8_t *> vars){
  unsigned long t;
  int count=vars.size();

  delayMicroseconds(_t_buf);

  t = micros();
  _wire.requestFrom(_addr, size_t(count), stopBit);
  for (auto b = vars.begin(); b != vars.end(); ++b) {
    if (_waitReadable(t)) {
      **b = _wire.read();
    } else {
      _rtc_errno = RTC_I2CR;
      return false;
    }
  }
  return true;
}

// end of i2c_rtc.cpp
