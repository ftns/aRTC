//
// aRTC/src/ds3231.cpp v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#include "ds3231.h"

bool ds3231::_init(){
  //  STAT_REG: clear all(OSF, EN32kHz, A2F, A1F);
  return _writeBytes(true, {DS3231_STAT_REG, 0x00});
}

bool ds3231::_checkValid(){
  uint8_t reg;

  // check OSF bit(DS3231_STAT_REG bit 7), 

  if (! _cmdReadBytes(DS3231_STAT_REG, false, {&reg})) {
    return false;
  }
  return ((reg & 0x80) == 0);
}


bool ds3231::_writeDateTime(rtc_tm *d){
  if (d->rtc_year<2000 || d->rtc_year>2099) {
    _rtc_errno = RTC_YEAR;
    return false;
  }
  return _writeBytes
    (true,
     { 0x00, // write from Reg.0
         byte2bcd(d->rtc_sec), // Reg.0: set seconds
         byte2bcd(d->rtc_min), // Reg.1: set minutes
         byte2bcd(d->rtc_hour), // Reg.2: set hours (in 24h mode)
         dayofweek(d->rtc_year, d->rtc_month, d->rtc_mday)+1,
         // Reg.3: set day of week
         // DS3231: 1=Sun, .. 7=Sat
         byte2bcd(d->rtc_mday), // Reg.4: set day of month
         byte2bcd(d->rtc_month), // Reg.5: set month
         byte2bcd(d->rtc_year - 2000)});// Reg.6: set year
}

//
bool ds3231::_readDateTime(rtc_tm *d){
  uint8_t rawSec, rawMin, rawHour, rawWDay, rawMDay, rawMonth, rawYear;

  if (!_cmdReadBytes
      (0x00, // from Reg.0
       false, // stopBit is off
       {&rawSec,// Reg.0: seconds
           &rawMin,   // Reg.1 minutes     
           &rawHour,   // Reg.2 hours
           &rawWDay, // Reg.3 day of week
           &rawMDay, // Reg.4 day of month
           &rawMonth,// Reg.5 month
           &rawYear}))// Reg.8 year
    return false;

  d->rtc_sec = bcd2byte(rawSec);
  d->rtc_min  = bcd2byte(rawMin);
  d->rtc_hour = bcd2byte(rawHour);
  d->rtc_wday = rawWDay - 1;
  d->rtc_mday = bcd2byte(rawMDay&0x3F); // remove Century bit
  d->rtc_month = bcd2byte(rawMonth);
  d->rtc_year = bcd2byte(rawYear)+2000;

  // check Century bit
  if ((rawMDay & 0x80) != 0){
    d->rtc_year += 100;
  }
  
  return true;
}


bool ds3231::getRTCtemp(int *t){
  uint8_t reg;

  if (!_cmdReadBytes(DS3231_TEMP_REG, true, {&reg})) return false;
  if ((reg & 0x80) == 0) {
    *t = reg;
  } else {
    *t = -(reg & 0x7F);
  }
  return true;
}


bool ds3231::getRTCtemp(float *t){
  uint8_t regT0, regT1;

  if (!_cmdReadBytes(DS3231_TEMP_REG, true, {&regT0, &regT1})) return false;
  *t = (regT0 & 0x7F) + ((regT1 >> 6)*0.25);
  if ((regT0 & 0x80) != 0) {
    *t *= -1;
  } 
  return true;
}  


// end of ds3231.cpp
//
