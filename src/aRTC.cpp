//
// src/aRTC/aRTC.cpp

#include "aRTC.h"
#include <time.h>

bool aRTC::startRTC(){
  noInterrupts();
  _valid = _checkValid();
  interrupts();

  if (_valid) return true;
    
  if (_rtc_errno == RTC_NOERR){
    bool result;

    delay(_POR_wait);
    noInterrupts();
    result = _init();
    interrupts();
    return result;
  } else {
    return false;
  }
} 


bool aRTC::setDateTime(int year, uint8_t month, uint8_t mday, uint8_t hour, uint8_t min, uint8_t sec){
  rtc_tm d;

  d.rtc_year = year;
  d.rtc_month = month;
  d.rtc_mday = mday;
  d.rtc_hour = hour;
  d.rtc_min = min;
  d.rtc_sec = sec;
  return setDateTime(&d);
}

bool aRTC::setDateTime(rtc_tm *d){
  uint8_t maxFebDay=28;
  
   _valid = false;

   //  d->rtc_year must be checked in _writeDateTime()

   if (d->rtc_month<1 || d->rtc_month>12) {
     _rtc_errno = RTC_MONTH;
     return false;
   }
   if (d->rtc_mday<1) {
     _rtc_errno = RTC_MDAY;
     return false;
   }
   if ((d->rtc_year % 4) == 0) maxFebDay = 29;
  
  switch (d->rtc_month){
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      if (d->rtc_mday > 31) {
        _rtc_errno = RTC_MDAY;
        return false;
      }
      break;
    case 4: case 6: case 9: case 11:
      if (d->rtc_mday > 30) {
        _rtc_errno = RTC_MDAY;
        return false;
      }
      break;
    case 2:
      if (d->rtc_mday > maxFebDay) {
        _rtc_errno = RTC_MDAY;
        return false;
      }
      break;
    default:
      _rtc_errno = RTC_MONTH;
      return false;
      break;
  } //switch

  noInterrupts();
  _valid = _writeDateTime(d);
  interrupts();

  return _valid;
}


bool aRTC::getDateTime(rtc_tm *d){
  bool result;

  noInterrupts();
  result = _readDateTime(d);
  interrupts();
  return (result && _valid);
}

bool aRTC::getDateTime(int *year, uint8_t *month, uint8_t *mday, uint8_t *wday, uint8_t *hour, uint8_t *min, uint8_t *sec){
  rtc_tm d;
  
  if (! getDateTime(&d)) {
    return false;
  }
  *year = d.rtc_year;
  *month = d.rtc_month;
  *mday = d.rtc_mday;
  *wday = d.rtc_wday;
  *hour = d.rtc_hour;
  *min = d.rtc_min;
  *sec = d.rtc_sec;
  return true;
}


// set epoch time to RTC
// NOTICE: epoch is local time, NOT UTC.
bool aRTC::setEpoch(time_t epoch){
  struct tm *t;
  rtc_tm d;
  
  t= localtime(&epoch);
  d.rtc_year = t->tm_year+1900;
  d.rtc_month = t->tm_mon+1;
  d.rtc_mday = t->tm_mday;
  d.rtc_hour = t->tm_hour;
  d.rtc_min = t->tm_min;
  d.rtc_sec = t->tm_sec;
  d.rtc_wday = t->tm_wday;

  return setDateTime(&d);
}

// get epoch time to RTC
// NOTICE: epoch is local time, NOT UTC.
time_t aRTC::getEpoch(){
  struct tm t;
  rtc_tm d;

  if (! getDateTime(&d)) return (time_t ) -1;
  t.tm_year = d.rtc_year-1900;
  t.tm_mon = d.rtc_month-1;
  t.tm_mday = d.rtc_mday;
  t.tm_hour = d.rtc_hour;
  t.tm_min = d.rtc_min;
  t.tm_sec = d.rtc_sec;
  t.tm_isdst = -1; // or 0?
  return mktime(&t);
}

// end of aRTC.cpp
