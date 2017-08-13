//
// aRTC/src/pcf_common.cpp v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#include "pcf_common.h"

bool pcf_common::_writeDateTime(rtc_tm *d){

  if (d->rtc_year<2001 || d->rtc_year>2099) {
    _rtc_errno = RTC_YEAR;
    return false;
  }
  return _writeBytes
    (true, // send STOP
     { uint8_t(PCF_COMMON_REG_SEC), // write from Reg.3
         byte2bcd(d->rtc_sec), // Reg.3: set seconds
         byte2bcd(d->rtc_min), // Reg.4: set minutes
         byte2bcd(d->rtc_hour), // Reg.5: set hours
         byte2bcd(d->rtc_mday), // Reg.6: set day of month
         dayofweek(d->rtc_year, d->rtc_month, d->rtc_mday),
         // Reg.7: set day of week
         byte2bcd(d->rtc_month), // Reg.8: set month
         byte2bcd(d->rtc_year-2000)}); // Reg.9: set year
}

//
bool pcf_common::_readDateTime(rtc_tm *d){
  uint8_t rawSec, rawMin, rawHour, rawMDay, rawWDay, rawMonth, rawYear;

  if (! _writeBytes(true, {uint8_t(PCF_COMMON_REG_SEC)})) return false;

  if (_readBytes
      (true, // send STOP
       { &rawSec, // Reg.3: read seconds & OSF bit
           &rawMin,  // Reg.4 read minutes
           &rawHour, // Reg.5 read hours
           &rawMDay, //. Reg.6 read day of month
           &rawWDay, //. Reg.7 read day of week
           &rawMonth, // Reg.8 read month
           &rawYear})) { // Reg. 9 read year

    d->rtc_min  = bcd2byte(rawMin);
    d->rtc_hour = bcd2byte(rawHour);
    d->rtc_wday = bcd2byte(rawWDay);
    d->rtc_mday = bcd2byte(rawMDay);
    d->rtc_month = bcd2byte(rawMonth);
    d->rtc_year = bcd2byte(rawYear)+2000;
    d->rtc_sec = bcd2byte(rawSec&0x7f);

    d->rtc_sec = bcd2byte(rawSec&0x7f);
    // return ((rawSec & 0x80) == 0); // check OSF bit
    // OSF flag is set when Vbss is failed but Vss is OK.
    return true;
  } else {
    return false;
  }
}


bool pcf_common::_checkValid(){
  uint8_t reg;

#if 0  
  if (!_writeBytes(true, {0x03})){
    _rtc_errno = RTC_I2CR;
    return false;
  }
  if (!_readBytes(true, {&reg})) return false;  
#endif
  
  if (! _cmdReadBytes(0x03, false, {&reg})) return false;
  
  return ((reg & 0x80) == 0) ; // check OSF bite
}

// end of pcf_common.cpp
