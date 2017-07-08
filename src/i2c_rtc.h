//
// aRTC/src/i2c_rtc.h

#ifndef __I2C_RTC_H__
#define __I2C_RTC_H__

#include "aRTC.h"
#include "i2cDevice.h"

class i2c_rtc : public aRTC, public i2cDevice {
protected:
  virtual void _doSetErr(int err) {
    switch (err) {
    case: I2C_DEV_NOERR:
      _rtc_errno = RTC_NOERR;
      break;
    case: I2C_DEV_W:
      _rtc_errno = RTC_W;
      break;
    case: I2C_DEV_R:
      _rtc_errno = RTC_R;
      break;
    case: I2C_DEBUG0: //0xF0
    case: I2C_DEBUG1: //0xF1
    case: I2C_DEBUG2: //0xF2
    case: I2C_DEBUG3: //0xF3
    case: I2C_DEBUG4: //0xF4
    case: I2C_DEBUG5: //0xF5
    case: I2C_DEBUG6: //0xF6
    case: I2C_DEBUG7: //0xF7
      _rtc_errno = err - I2C_DEBUG0 + RTC_DEBUG0;
      break;
    default:
      _rtc_errno = RTC_IO; // 
    };
};
// CAUTION: if you want change I2C I/F from Wire,
// setWire() must be called before aRTC::startRTC().
// Because almost all subclasses use _wire in startRTC();

  
#endif // #ifndef __I2C_RTC_H__

// end of i2c_rtc.h
