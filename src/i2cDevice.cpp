//
// aRTC/src/i2cDevice.cpp v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#include "i2cDevice.h"

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

#ifdef I2CwRetry
  unsigned long t;

  t = micros();

  // loop until timeout(default 10ms=10000micro sec)
  while (diffULong(micros(), t) < _timeout) {
#endif
    delayMicroseconds(_t_buf);

    _wire.beginTransmission(_addr);
    _wire.write(area, nbytes);
    
    err = _wire.endTransmission(stopBit);

#ifdef I2CwRetry
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
#else
  if (err == 0) {
    return true;
  } else {
    setErr(I2C_DEV_W);
    return false;
  }
#endif
}

//
bool i2cDevice::_writeBytes(bool stopBit, std::initializer_list<const uint8_t> data){
  int err;

#ifdef I2CwRetry
  unsigned long t;
  t = micros();
  // loop until timeout(default 10ms=10000micro sec)
  while (diffULong(micros(), t) < _timeout) {
#endif
    delayMicroseconds(_t_buf);

    _wire.beginTransmission(_addr);

    for (auto b = data.begin(); b != data.end(); ++b){
      _wire.write(*b);
    }
    
    err = _wire.endTransmission(stopBit);

#ifdef I2CwRetry
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
#else
  if (err == 0) {
    return true;
  } else {
    setErr(I2C_DEV_W);
    return false;
  }
#endif
}


bool i2cDevice::_readBytes(bool stopBit, uint8_t nbytes, uint8_t *area){
  unsigned long t;

  delayMicroseconds(_t_buf);

  t = micros();
  _wire.requestFrom(_addr, size_t(nbytes), stopBit);
  for (uint8_t i=0; i<nbytes; i++) {
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
  unsigned long t;
  int count=vars.size();
  
  delayMicroseconds(_t_buf);

  _wire.beginTransmission(_addr);
  _wire.write(cmd);
  if (_wire.endTransmission(stopBit)!=0){
    setErr(I2C_DEV_R);
    return false;
  }
  
  t = micros();
  _wire.requestFrom(_addr, size_t(count), true); // send STOP
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


bool i2cDevice::_cmdReadBytes(uint8_t cmd, bool stopBit, uint8_t nbytes, uint8_t *area){
  unsigned long t;

  if (! _writeBytes(stopBit, {cmd})) {
    setErr(I2C_DEV_R);
    return false;
  }
  return _readBytes(true, nbytes, area);
}

// cmdWriteBytes
bool i2cDevice::_cmdWriteBytes(uint8_t cmd, uint8_t nbytes, uint8_t *area){
  delayMicroseconds(_t_buf);
  _wire.beginTransmission(_addr);
  _wire.write(cmd);
  _wire.write(area, nbytes);
  return (_wire.endTransmission()==0);
}

bool i2cDevice::_cmdWriteBytes(uint8_t cmd, std::initializer_list<const uint8_t> data){
  delayMicroseconds(_t_buf);

  _wire.beginTransmission(_addr);
  _wire.write(cmd);
  
  for (auto b = data.begin(); b != data.end(); ++b){
    _wire.write(*b);
  }

  return (_wire.endTransmission()==0);
}

// end of i2cDevice.cpp
