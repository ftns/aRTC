//
// aRTC/src/ds1307.cpp v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#include "ds1307.h"

bool ds1307::_init(){
  // clear control register
  return _writeBytes
    (true,
     {0x07, 0x00});// // Reg.07h, all clear
}

bool ds1307::_checkValid(){
  rtc_tm d;
  
  // get date, time, CH bit
  if (! _readDateTime(&d)) return false;
  
  // check power failure (experimental)
  // if 2000/1/1 00:00, perhaps power was failed(resetted)
  return !((d.rtc_year==2000)&&
           (d.rtc_month==1)&&
           (d.rtc_mday==1)&&
           (d.rtc_hour==0)&&
           (d.rtc_min==0));
}


bool ds1307::_writeDateTime(rtc_tm *d){
  if (d->rtc_year<2000 || d->rtc_year>2099) {
    _rtc_errno = RTC_YEAR;
    return false;
  }
  return _writeBytes
    (true,
     { 0x00, // write from Reg.0
         byte2bcd(d->rtc_sec), // Reg.0: start clock & set seconds
         byte2bcd(d->rtc_min), // Reg.1 set minutes
         byte2bcd(d->rtc_hour), // Reg.2: set hours (in 24h mode)
         dayofweek(d->rtc_year, d->rtc_month, d->rtc_mday)+1,
         // Reg.3: set day of week
         // DS1307: 1=Sun, .. 7=Sat
         byte2bcd(d->rtc_mday), // Reg.4: set day of month
         byte2bcd(d->rtc_month), // Reg.5: set month
         byte2bcd(d->rtc_year - 2000)});// Reg.6: set year
}


bool ds1307::isRunning(){
  uint8_t reg;

  if (!_cmdReadBytes(0x00, false, {&reg})) return false;
  
  return ((reg&0x80)==0); // Reg.0: read seconds & CH bit
}

bool ds1307::halt(){
  return _setCH(true);
}

bool ds1307::restart(){
  return _setCH(false);
}

bool ds1307::_setCH(bool ch){
  uint8_t rawSec;
  bool prev;

  if (! _cmdReadBytes(0x00, false, {&rawSec})) return false; 
  
  prev = ((rawSec&0x80) != 0);
  if (prev != ch) {
    if (ch) {
      if (!_writeBytes(true, {(rawSec | 0x80)})) return false;
    } else {
      if (!_writeBytes(true, {(rawSec & 0x7F)})) return false;
    }
  }
  return true;
}

//
bool ds1307::_readDateTime(rtc_tm *d){
  uint8_t rawSec, rawMin, rawHour, rawWDay, rawMDay, rawMonth, rawYear;

  if (!_cmdReadBytes
      (0x00, // from 0x00
       false, // stopBit is off
       {&rawSec,// Reg.0: seconds & CH bit
           &rawMin,   // Reg.1 minutes     
           &rawHour,   // Reg.2 hours
           &rawWDay, // Reg.3 day of week
           &rawMDay, // Reg.4 day of month
           &rawMonth,// Reg.5 month
           &rawYear}))// Reg.8 year
    return false;

  d->rtc_sec = bcd2byte(rawSec & 0x7F);
  d->rtc_min  = bcd2byte(rawMin);
  d->rtc_hour = bcd2byte(rawHour);
  d->rtc_wday = rawWDay - 1;
  d->rtc_mday = bcd2byte(rawMDay&0x3F);
  d->rtc_month = bcd2byte(rawMonth);
  d->rtc_year = bcd2byte(rawYear)+2000;
  d->rtc_sec = bcd2byte(rawSec & 0x7F);

  return ((rawSec & 0x80) == 0); // check CH bit
}

//
// nvram
//
bool ds1307::nvramWrite(uint8_t r_addr, uint8_t len, uint8_t *area){
  if (len == 0) return true;
  if ((len+r_addr) > DS1307_NVRAM_LEN) {
    _rtc_errno = RTC_PARAM;
    return false;
  }
  // set first reg. addr.
  r_addr += DS1307_NVRAM_ADDR;

  return _cmdWriteBytes(r_addr, len, area);
}

bool ds1307::nvramWrite(uint8_t r_addr, std::initializer_list<const uint8_t> values){
  uint8_t len = values.size();
  if (len == 0) return true;
  if ((len+r_addr) > DS1307_NVRAM_LEN) {
    _rtc_errno = RTC_PARAM;
    return false;
  }
  // set first reg. addr.
  r_addr += DS1307_NVRAM_ADDR;
  return _cmdWriteBytes(r_addr, values);
}

//
bool ds1307::nvramRead(uint8_t r_addr, uint8_t len, uint8_t *area){  
  if (len == 0) return true;    
  if ((len+r_addr) > DS1307_NVRAM_LEN) {
    _rtc_errno = RTC_PARAM;
    return false;
  }

  r_addr += DS1307_NVRAM_ADDR;
  return _cmdReadBytes(r_addr, false, len, area);
}

//
bool ds1307::nvramRead(uint8_t r_addr, std::initializer_list<uint8_t *> vars){
  uint8_t len = vars.size();
  if (len == 0) return true;    
  if ((r_addr+len) > DS1307_NVRAM_LEN) {
    _rtc_errno = RTC_PARAM;
    return false;
  }

  r_addr += DS1307_NVRAM_ADDR;
  return _cmdReadBytes(r_addr, false, vars);
}

// end of DS1307.cpp
//
