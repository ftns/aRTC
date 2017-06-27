//$Id: rx8025.cpp,v 1.3 2017/06/25 13:45:31 akihiro Exp akihiro $
// rx8025.cpp

#include "rx8025.h"

bool rx8025::_writeDateTime(rtc_tm *d){
  if (d->rtc_year<2001 || d->rtc_year>2099) {
    rtc_errno = RTC_YEAR;
    return false;
  }
  return _writeBytes
    (true, // send STOP bit
     {0x03, // write from Reg.3
         byte2bcd(d->rtc_sec),  // Reg.3: set seconds
         byte2bcd(d->rtc_min),  // Reg.4 set minutes
         byte2bcd(d->rtc_hour), // Reg.5: set hours
         byte2bcd(d->rtc_mday), // Reg.6: set day of month
         dayofweek(d->rtc_year, d->rtc_month, d->rtc_mday),
         // Reg.7: set day of week
         byte2bcd(d->rtc_month), // Reg.8: set month
         byte2bcd(d->rtc_year-2000)}); // Reg.9: set year
}

//
bool rx8025::_readDateTime(rtc_tm *d){
  uint8_t rawSec, rawMin, rawHour, rawWDay, rawMDay, rawMonth, rawYear;

  if (!_writeBytes(false, {0x00})) { // read from Reg.00
    rtc_errno = RTC_I2CR;       
    return false;
  }

  if (!_readBytes
      (true, 
       {&rawSec,      // Reg.0: seconds
           &rawMin,   // Reg.1 minutes
           &rawHour,  // Reg.2 hours
           &rawWDay,  // Reg.3 day of week
           &rawMDay,  // Reg.4 day of month
           &rawMonth, // Reg.5 monthy & century bit
           &rawYear})) { // Reg.6 year
    rtc_errno = RTC_I2CR;
    return false;
  }    

  d->rtc_sec = bcd2byte(rawSec);
  d->rtc_min  = bcd2byte(rawMin&0x7F); 
  d->rtc_hour = bcd2byte(rawHour&0x3F);
  d->rtc_wday = rawWDay;
  d->rtc_mday = bcd2byte(rawMDay&0x3F); 
  //rawMonth  = _wire.read(); 
  d->rtc_year = bcd2byte(rawYear)+2000;

  // check centry bit
  if ((rawMonth & 0x80) == 0){
    d->rtc_month=bcd2byte(rawMonth);
  } else {
    d->rtc_month=bcd2byte(rawMonth&0x1f);
    d->rtc_year+=100;
  }
  return true;
}


bool rx8025::_checkValid(){
  uint8_t reg;
  
  if (!_writeBytes(false, {0x00})) {// read from registor 0F
    rtc_errno = RTC_I2CR;
    return false;
  }

  if (!_readBytes(true, {&reg})) {
    rtc_errno = RTC_I2CR;
    return false;
  }
    
  return ((reg & 0x30) == 0x20); // /XST=1 & PON = 0
}

bool rx8025::_init(){

  // initialize Control Register
  if (! _writeBytes
      (true,
       {0x0E, // write from registor 0E
           0x30, // Reg.0E: Alarm off, 24h mode, /CLEN2=1, Interval Timer off
           0x00})) {// Reg.0F: Vthreshold = 1.3V, VDET=/XST=PON=0, /CLEN1=1, in
    rtc_errno = RTC_I2CW;
    return false;
  }

  // initialize date, time, offset, alarm
  if (! _writeBytes
      (true,
       {0x00, // write from registor 00
           // set to 0:00:00
           0x00, // Reg.0: seconds by BCD
           0x00, // Reg.1: minutes by BCD
           0x00, // Reg.2: hours by BCD
           // set to 2017/1/1 (Sun.)
           0x00, // Reg.3: day of week: Sunday
           0x01, // Reg.4: day of month by BCD (& Century bit clear)
           0x01, // Reg.5: month by BCD
           0x17, // Reg.6: Years by BCD
           // offset
           0x00, // Reg.7: clock offset
           // Weekly Alarm  
           0x00, // Reg.8: minute
           0x00, // Reg.9: hour
           0x00, // Reg.0A: bitmask of day of week: disarmed
           // Daily Alarm
           0x00, // Reg.0B: minute
           0x00})) { // Reg.0C: hour
    rtc_errno = RTC_I2CW;
    return false;
  }
  return true;
}


// end of rx8025.cpp
//
