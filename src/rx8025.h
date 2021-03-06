//
// aRTC/src/rx8025.h v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#ifndef __RX8025_H__
#define __RX8025_H__

#include <Wire.h>
#include "i2c_rtc.h"
#include <time.h>

#define RX8025_ADDR 0x32
#define RX8025_T_BUF 80 // 62 micro sec
#define RX8025_POR_T 1200 // 1sec

class rx8025 : public i2c_rtc {
public:
  rx8025(){
    setPORwait(RX8025_POR_T);
    setADDR(RX8025_ADDR);
    setT_BUF(RX8025_T_BUF);
  }
private:
  bool _writeDateTime(rtc_tm *d);
  bool _readDateTime(rtc_tm *d);
  bool _checkValid();
  bool _init();
};

#endif // ifndef __RX8025_H__

// end of rx8025.h
