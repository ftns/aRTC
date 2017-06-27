//$Id: aRTC_util.h,v 1.2 2017/06/24 13:33:15 akihiro Exp $

#include <stdint.h>

#ifndef __aRTC_UTIL_H__
#define __aRTC_UTIL_H__ 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  extern uint8_t byte2bcd(uint8_t b);
  extern uint8_t bcd2byte(uint8_t b);
  extern uint8_t dayofweek(int y, int m, int d);
  extern unsigned long diffULong(unsigned long x, unsigned long y);  

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef __aRTC_UTIL_H__ */

