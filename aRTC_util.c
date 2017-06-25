//$Id: aRTC_util.c,v 1.2 2017/06/25 13:45:31 akihiro Exp akihiro $
// aRTC_util.c
#include "aRTC_util.h"


uint8_t bcd2byte(uint8_t b) {
  return ((b & 0xf0) >> 4)*10 + (b & 0x0f);
}

uint8_t byte2bcd(uint8_t b){
  return ((b / 10)*0x10 + (b % 10));
}


unsigned long diffULong(unsigned long x, unsigned long y){
  long long lx=x, ly=y, lresult;
  lresult = lx-ly;
  if (lresult < 0) {
    return (unsigned long)(lresult + ((unsigned long)-1L) + 1);
  } else {
    return (unsigned long)(lresult);
  }
}


    
// Zeller's Congruence
uint8_t dayofweek(int year, int month, int day){
  int c, y;

  if (month < 3) {
    year --;
    month +=12;
  }    
  c = year/100;
  y = year % 100;
  
  return (day + (13*(month+1))/5 + y + y/4 + c/4 + 5*c +6)%7;
}

// end of aRTC_util.c
//
