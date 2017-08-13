//
// aRTC/src/rtc8564.cpp v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#include "rtc8564.h"

bool rtc8564::_writeDateTime(rtc_tm *d){

  if (d->rtc_year<2001 || d->rtc_year>2099) {
    _rtc_errno = RTC_YEAR;
    return false;
  }

  // stop clock
  if (! _writeBytes
      (true,  // send STOP bit
       {0x00, // write from registor 00
           0x20})){ // Reg.0: stop clock
    //    _rtc_errno = RTC_I2CW;
    return false;
  }    

  // set sec, min , hour, day of month/week, month, year
  if (! _writeBytes
      (true,  // send STOP bit
       {0x02, // write from Reg.2
           byte2bcd(d->rtc_sec),  // Reg.2: set seconds and VL bit clear
           byte2bcd(d->rtc_min),  // Reg.3 set minutes
           byte2bcd(d->rtc_hour), // Reg.4: set hours
           byte2bcd(d->rtc_mday), // Reg.5: set day of month
           dayofweek(d->rtc_year, d->rtc_month, d->rtc_mday),
                                  // Reg.6: set day of week
           byte2bcd(d->rtc_month),       // Reg.7: set month
           byte2bcd(d->rtc_year-2000)})) {     // Reg.8: set year
    // _rtc_errno = RTC_I2CW;
    return false;
  }    

  // restart clock
  if (! _writeBytes
    (true,  // send STOP bit
     {0x00, // write from registor 00
      0x00})){ // Reg.0: start clock
    // _rtc_errno = RTC_I2CW;
    return false;
  }
  return true;
}

//
bool rtc8564::_readDateTime(rtc_tm *d){
  uint8_t rawSec, rawMin, rawHour, rawMDay, rawWDay, rawMonth, rawYear;
  bool result;

  if (!_cmdReadBytes
      (0x02,             // read from Reg.2
       false,            // do not send STOP between write & read
       {&rawSec,         // Reg.2 second
           &rawMin,      // Reg.3 read minutes
           &rawHour,     // Reg.4 read hours
           &rawMDay,     // Reg.5 read day of month
           &rawWDay,     //. Reg.6 read day of week
           &rawMonth,    // Reg.7 read month & century bit
           &rawYear})) { // Reg. 8 read year
    // _rtc_errno = RTC_I2CR;
    return false;
  }

  d->rtc_min  = bcd2byte(rawMin&0x7F);
  d->rtc_hour = bcd2byte(rawHour&0x3F);
  d->rtc_mday = bcd2byte(rawMDay&0x3F);
  d->rtc_wday = rawWDay & 0x07;
  rawMonth = rawMonth&0x9F;
  d->rtc_year = bcd2byte(rawYear)+2000;

  // check VL bit
  result = ((rawSec & 0x80) == 0);
  d->rtc_sec = bcd2byte(rawSec & 0x7F);
  
  // check centry bit
  if ((rawMonth & 0x80) == 0){
    d->rtc_month=bcd2byte(rawMonth);
  } else {
    d->rtc_month=bcd2byte(rawMonth&0x7f);
    d->rtc_year+=100;
  }
  return result;  
}

bool rtc8564::_checkValid(){
  uint8_t result;

  if (!_cmdReadBytes(0x02, false, {&result})) return false;

  return ((result & 0x80) == 0);
}

bool rtc8564::_init(){
  if (!_writeBytes
      (true, // send STOP bit
       {0x00, // write from registor 00
           0x20, // Reg.0: stop clock
           0x00, // Reg.1: stop all interrupt
           // set to 0:00:00
           0x00, // Reg.2: seconds by BCD
           0x00, // Reg.3: minutes by BCD
           0x00, // Reg.4: hours by BCD
           // set to 2017/1/1 (Sun.)
           0x01, // Reg.5: day of month by BCD
           0x00, // Reg.6: day of week: Sunday
           0x01, // Reg.7: Century bit clear & month by BCD
           0x17, // Reg.8: Years by BCD
           0x80, // Reg.9: Minutes Alarm disarmed(AE=1)
           0x80, // Reg.A: Hour Alam disarmed(AE=1)
           0x80, // Reg.B: Day of Month Alam disarmed(AE=1)
           0x80, // Reg.C: Day of Week Alam disarmed(AE=1)
           0x00, // Reg.D: Clockout disabled
           0x00, // Reg.E: Interval Timer disabled.
           0xFF})) { // Reg.F: Interval Timer cound(max).
    // _rtc_errno = RTC_I2CW;
    return false;
  }

  if (!_writeBytes
      (true, // send STOP bit
       {0x00, // write from registor 00
           0x00})) { // Reg.0: start clock
    // _rtc_errno = RTC_I2CW;
    return false;
  }
  return true;
}


// end of rtc8564.cpp
//
