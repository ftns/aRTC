//$Id: rtc8564.h,v 1.5 2017/06/24 13:33:15 akihiro Exp $
// rtc8564.h

#ifndef __RTC8564_H__
#define __RTC8564_H__

#include <Wire.h>
#include "i2c_rtc.h"
#include <time.h>

#define RTC8564_ADDR 0x51
#define RTC8564_T_BUF 2 // 1.3 micro sec
#define DEFAULT_STARTUP_T 1000 // 1sec

class rtc8564 : public i2c_rtc {
public:
  rtc8564(){
    _POR_wait = DEFAULT_STARTUP_T; // at least 1sec;
    _addr = RTC8564_ADDR;
    _t_buf = RTC8564_T_BUF;
  }
  rtc8564(uint16_t POR_wait){
    _POR_wait = POR_wait;
    _addr = RTC8564_ADDR;
    _t_buf = RTC8564_T_BUF;
  }
  rtc8564(uint16_t POR_wait, uint32_t I2C_timeout){
    _POR_wait = POR_wait;
    _timeout = I2C_timeout;
    _addr = RTC8564_ADDR;
    _t_buf = RTC8564_T_BUF;
  }

private:
  bool _writeDateTime(rtc_tm *d);
  bool _readDateTime(rtc_tm *d);
  bool _checkValid();
  bool _init();
};

#endif // ifndef __RTC8564_H__

// end of RTC8564.h
