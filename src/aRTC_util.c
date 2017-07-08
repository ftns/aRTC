//
// aRTC/src/aRTC_util.c
#include "aRTC_util.h"


uint8_t bcd2byte(uint8_t b) {
  return ((b & 0xf0) >> 4)*10 + (b & 0x0f);
}

uint8_t byte2bcd(uint8_t b){
  return ((b / 10)*0x10 + (b % 10));
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
