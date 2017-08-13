//
// aRTC/src/aRTC.h v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#ifndef __aRTC_H__
#define __aRTC_H__

#include <Wire.h>
#include "aRTC_util.h"
#include <Arduino.h>
#include <time.h>

#define _aRTC_DEBUG_ 1

// error code
#define RTC_NOERR 0
#define RTC_NOFEATURE 1
#define RTC_NOT_IMPLEMENTED 2
#define RTC_ERR 3

#define RTC_PARAM 0x10
#define RTC_YEAR  0x11
#define RTC_MONTH 0x12
#define RTC_MDAY  0x13
#define RTC_HOUR  0x14
#define RTC_MIN   0x15
#define RTC_SEC   0x16

#define RTC_IO 0x20
#define RTC_W  0x21
#define RTC_R  0x22

#define RTC_DEBUG0 0x30
#define RTC_DEBUG1 0x31
#define RTC_DEBUG2 0x32
#define RTC_DEBUG3 0x33
#define RTC_DEBUG4 0x34
#define RTC_DEBUG5 0x35
#define RTC_DEBUG6 0x36
#define RTC_DEBUG7 0x37

struct rtc_tm {  // date&time struct for RTC
  uint8_t rtc_sec;   // seconds [0..59]
  uint8_t rtc_min;   // minuts [0..59]
  uint8_t rtc_hour;  // hours [0..23]
  uint8_t rtc_wday;  // day of week [0=Sunday..6]
  uint8_t rtc_mday;  // day of month [1..31]
  uint8_t rtc_month; // month [1..12]
  int rtc_year;  // day of year
};
//< NOTICE:
// The range of rtc_year may depend on RTC chip.
// However, almost all RTCs are 2001-2099.

class aRTC {
public:
  virtual int rtc_errno(){ return _rtc_errno; };
  bool startRTC();
  bool setDateTime(rtc_tm *d);
  bool setDateTime(int year, uint8_t month, uint8_t mday, uint8_t hour, uint8_t min, uint8_t sec);
  bool getDateTime(rtc_tm *d);
  bool getDateTime(int *year, uint8_t *month, uint8_t *mday, uint8_t *wday, uint8_t *hour, uint8_t *min, uint8_t *sec);
  bool setEpoch(time_t epoch);
  time_t getEpoch();
  bool isValid(){
    return _valid;
  };
  virtual bool resetErrno(){
    _rtc_errno = RTC_NOERR;
    return true;
  };
  void setPORwait(uint16_t t) { _POR_wait = t; };

protected:
  uint16_t _POR_wait = 0; // msec.
  int _rtc_errno = RTC_NOERR; // error code
  
private:
  bool _valid = false;

  virtual bool _init(){ return true; };
  virtual bool _writeDateTime(rtc_tm *d){
    _rtc_errno = RTC_NOT_IMPLEMENTED;
    return false;
  };
  virtual bool _readDateTime(rtc_tm *d){
    _rtc_errno = RTC_NOT_IMPLEMENTED;
    return false;
  };
  virtual bool _checkValid(){ return true; };
};

#endif // ifndef __aRTC_H__

// end of aRTC.h
