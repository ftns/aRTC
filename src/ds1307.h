//
// aRTC/src/ds1307.h v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#ifndef __DS1307_H__
#define __DS1307_H__

#include <Wire.h>
#include "i2c_rtc.h"
#include <time.h>

#define DS1307_ADDR 0x68
#define DS1307_T_BUF 5 // 4.7 micro sec
#define DS1307_NVRAM_LEN 56
#define DS1307_NVRAM_ADDR 0x08
#define DS1307_LASTADDR 0x3f
#define DS1307_POR_T 0

class ds1307 : public i2c_rtc {
public:
  ds1307(){
    setPORwait(DS1307_POR_T);
    setADDR(DS1307_ADDR);
    setT_BUF(DS1307_T_BUF);
  };
  bool isRunning();
  bool halt();
  bool restart();

  // nvram read &write
  // r_addr = 0-55(0x37)
  bool nvramRead(uint8_t r_addr, uint8_t len, uint8_t *area);
  bool nvramRead(uint8_t r_addr, std::initializer_list<uint8_t *> vars);
  bool nvramWrite(uint8_t r_addr, uint8_t len, uint8_t *area);
  bool nvramWrite(uint8_t r_addr, std::initializer_list<const uint8_t> values);
                  
private:
  bool _init();
  bool _checkValid();
  bool _setCH(bool ch);
  bool _writeDateTime(rtc_tm *d);
  bool _readDateTime(rtc_tm *d);
};

#endif // ifndef __DS1307_H__

// end of ds1307.h
