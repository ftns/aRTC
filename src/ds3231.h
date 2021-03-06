// 
// aRTC/src/ds3231.h v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#ifndef __DS3231_H__
#define __DS3231_H__

#include <Wire.h>
#include "i2c_rtc.h"
#include <time.h>

#define DS3231_ADDR 0x68

#define DS3231_CNTL_REG 0x0E
#define DS3231_STAT_REG 0x0F
#define DS3231_TEMP_REG 0x11

#define DS3231_T_BUF 5 // 4.7 micro sec

class ds3231 : public i2c_rtc {
public:
  ds3231(){
    _POR_wait = 0;
    _setADDR(DS3231_ADDR);
    _setT_BUF(DS3231_T_BUF);
  };

  bool getRTCtemp(int *t);
  bool getRTCtemp(float *t);
  
private:
  bool _init();
  bool _checkValid();
  bool _writeDateTime(rtc_tm *d);
  bool _readDateTime(rtc_tm *d);
};

#endif // ifndef __DS3231_H__

// end of ds3231.h
