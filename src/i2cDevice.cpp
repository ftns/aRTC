//
// aRTC/src/i2cDevice.cpp

#include "i2cDevice.h"

//
// Notice:
//   _writeBytes(), _readBytes(), _cmdReadbytes() has code duplication.
//   These should be removed, however, how to do without memory 
//   consumpion? This problem is due to variale argument.
//

bool i2cDevice::_waitReadable(unsigned long start){
  while (! _wire.available()){
    if (diffULong(micros(), start)>_timeout) {
      setErr(I2C_DEV_R);// _rtc_errno = RTC_I2CR;
      return false;
    };
    delayMicroseconds(10);
  }
  return true;
}

//
// _writeBytes
// 
bool i2cDevice::_writeBytes(bool stopBit, uint8_t nbytes, uint8_t *area){
  int err;
  unsigned long t;

  t = micros();

  // loop until timeout(default 10ms=10000micro sec)
  while (diffULong(micros(), t) < _timeout) {
    delayMicroseconds(_t_buf);

    _wire.beginTransmission(_addr);

    for (uint8_t i = 0; i<nbytes; i++)
      _wire.write(area[i]);
    
    err = _wire.endTransmission(stopBit);

    if (err == 0) {
      return true;
    } else if (err != 2) {
      setErr(I2C_DEV_W); //_rtc_errno = RTC_I2CW;
      return false;
    }

    // if err==2, rtc seems to be busy? (or network error?)
    delayMicroseconds(5000); // wait 5msec
  } // while

  setErr(I2C_DEV_W); // _rtc_errno = RTC_I2CW;
  return false;
}

//
bool i2cDevice::_writeBytes(bool stopBit, std::initializer_list<const uint8_t> data){
  int err;
  unsigned long t;

  t = micros();

  // loop until timeout(default 10ms=10000micro sec)
  while (diffULong(micros(), t) < _timeout) {
    delayMicroseconds(_t_buf);

    _wire.beginTransmission(_addr);

    for (auto b = data.begin(); b != data.end(); ++b){
      _wire.write(*b);
    }
    
    err = _wire.endTransmission(stopBit);

    if (err == 0) {
      return true;
    } else if (err != 2) {
      setErr(I2C_DEV_W); // _rtc_errno = RTC_I2CW;
      return false;
    }

    // if err==2, rtc seems to be busy? (or network error?)
    delayMicroseconds(5000); // wait 5msec
  } // while

  setErr(I2C_DEV_W); // _rtc_errno = RTC_I2CW;
  return false;
}


bool i2cDevice::_readBytes(bool stopBit, uint8_t nbytes, uint8_t *area){
  unsigned long t;

  delayMicroseconds(_t_buf);

  t = micros();
  _wire.requestFrom(_addr, size_t(nbytes), stopBit);
  for (int i=0; i<nbytes; i++) {
    if (_waitReadable(t)) {
      area[i] = _wire.read();
    } else {
      setErr(I2C_DEV_R); // _rtc_errno = RTC_I2CR;
      return false;
    }
  }
  return true;
}


bool i2cDevice::_readBytes(bool stopBit, std::initializer_list<uint8_t *> vars){
  unsigned long t;
  int count=vars.size();

  delayMicroseconds(_t_buf);

  t = micros();
  _wire.requestFrom(_addr, size_t(count), stopBit);
  for (auto b = vars.begin(); b != vars.end(); ++b) {
    if (_waitReadable(t)) {
      **b = _wire.read();
    } else {
      setErr(I2C_DEV_R);// _rtc_errno = RTC_I2CR;
      return false;
    }
  }
  return true;
}


bool i2cDevice::_cmdReadBytes(uint8_t cmd, bool stopBit, std::initializer_list<uint8_t *> vars){

  if (! _writeBytes(stopBit, {cmd})) {
    setErr(I2C_DEV_R); // _rtc_errno = RTC_I2CR;
    return false;
  }
  return _readBytes(true, vars);
}


bool i2cDevice::_cmdReadBytes(uint8_t cmd, bool stopBit, uint8_t nbytes, uint8_t *area){
  unsigned long t;

  if (! _writeBytes(stopBit, {cmd})) {
    setErr(I2C_DEV_R); // _rtc_errno = RTC_I2CR;
    return false;
  }
  return _readBytes(true, nbytes, area);
}


// end of i2cDevice.cpp
