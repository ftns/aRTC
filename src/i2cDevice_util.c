//
// aRTC/src/i2cDevice_util.c

#include "i2cDevice_util.h"

unsigned long diffULong(unsigned long x, unsigned long y){
  long long lx=x, ly=y, lresult;
  lresult = lx-ly;
  if (lresult < 0) {
    return (unsigned long)(lresult + ((unsigned long)-1L) + 1);
  } else {
    return (unsigned long)(lresult);
  }
}

// end of i2cDevice_util.c
