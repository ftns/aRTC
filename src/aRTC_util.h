//
// aRTC/src/aRTC_util.h v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#include <stdint.h>

#ifndef __aRTC_UTIL_H__
#define __aRTC_UTIL_H__ 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  extern uint8_t byte2bcd(uint8_t b);
  extern uint8_t bcd2byte(uint8_t b);
  extern uint8_t dayofweek(int y, int m, int d);
  //  extern unsigned long diffULong(unsigned long x, unsigned long y);  

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef __aRTC_UTIL_H__ */

