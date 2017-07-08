//
// aRTC/src/rtc8564.h

#ifndef __RTC8564_H__
#define __RTC8564_H__

#include <Wire.h>
#include "i2c_rtc.h"
#include <time.h>

#define RTC8564_ADDR 0x51
#define RTC8564_T_BUF 2 // 1.3 micro sec
#define RTC8564_POR_T 1000 // 1sec

class rtc8564 : public i2c_rtc {
public:
  rtc8564(){
    setPORwait(RTC8564_POR_T);
    setADDR(RTC8564_ADDR);
    setT_BUF(RTC8564_T_BUF);
  }
  rtc8564(uint16_t POR_wait){
    rtc8564();
    setPORwait(POR_wait);
  }
  rtc8564(uint16_t POR_wait, uint32_t I2C_timeout){
    rtc8564();
    setPORwait(POR_wait);
    setTimeout(I2C_timeout);
  }

private:
  bool _writeDateTime(rtc_tm *d);
  bool _readDateTime(rtc_tm *d);
  bool _checkValid();
  bool _init();
};

#endif // ifndef __RTC8564_H__

// end of rtc8564.h
