//
// aRTC/src/i2cDevice.h v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#ifndef __I2C_DEVICE_H__
#define __I2C_DEVICE_H__

#include <Arduino.h>
#include <Wire.h>
#include <initializer_list>
#include "i2cDevice_util.h"

#define I2C_DEV_NOERR 0x00
#define I2C_DEV_ERR   0xE0
#define I2C_DEV_W     0xE1
#define I2C_DEV_R     0xE2

#define I2C_DEV_DEBUG0 0xF0
#define I2C_DEV_DEBUG1 0xF1
#define I2C_DEV_DEBUG2 0xF2
#define I2C_DEV_DEBUG3 0xF3
#define I2C_DEV_DEBUG4 0xF4
#define I2C_DEV_DEBUG5 0xF5
#define I2C_DEV_DEBUG6 0xF6
#define I2C_DEV_DEBUG7 0xF7

//#define I2CwRetry 1 // retry I2C write when address error occures

class i2cDevice {
public:
  void setWire(TwoWire w){
    _wire = w;
  };
  int i2c_errno() { return _i2c_errno; };

  void setADDR(uint8_t addr) { _addr = addr; };
  // I2C address(7bit)
  void setT_BUF(uint32_t t) { _t_buf = t; };
  // T_BUF: BUs Free Time between STOP and START condition
  // (microseconds)
  void setTimeout(uint32_t t) { _timeout = t; };
  // wait device until timeout(microseconds)
  
protected:
  TwoWire _wire = Wire;
  // TwoWire(I2C) interface
  uint8_t _addr;
  // I2C address(7bit)
  
  uint32_t _timeout = 10000; // default 10ms = 10,000 micro sec
  uint32_t _t_buf = 100; // default: 100 micro sec

  bool _waitReadable(unsigned long start);
  
  bool _writeBytes(bool stopBit, std::initializer_list<const uint8_t> data);
  bool _writeBytes(bool stopBit, uint8_t nbytes, uint8_t *area);

  bool _readBytes(bool stopBit, std::initializer_list<uint8_t *> vars);
  bool _readBytes(bool stopBit, uint8_t nbytes, uint8_t *area);

  bool _cmdReadBytes(uint8_t cmd, bool stopBit, std::initializer_list<uint8_t *> vars);
  bool _cmdReadBytes(uint8_t cmd, bool stopBit, uint8_t nbytes, uint8_t *area);

  bool _cmdWriteBytes(uint8_t cmd, uint8_t nbytes, uint8_t *area);
  bool _cmdWriteBytes(uint8_t cmd, std::initializer_list<const uint8_t> data);
  
  // error handling
  void setErr(int err) {
    _i2c_errno = err;
    _doSetErr(err);
  };
  virtual void _doSetErr(int err) {};

private:
  int _i2c_errno = I2C_DEV_NOERR;
};

#endif // #ifndef __I2C_DEVICE_H__

// end of i2cDevice.h
