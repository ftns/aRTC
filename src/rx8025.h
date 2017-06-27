//$Id: rx8025.h,v 1.3 2017/06/24 13:33:15 akihiro Exp $
// rx8025.h

#ifndef __RX8025_H__
#define __RX8025_H__

#include <Wire.h>
#include "i2c_rtc.h"
#include <time.h>

#define RX8025_ADDR 0x32
#define RX8025_T_BUF 80 // 62 micro sec(min)
#define DEFAULT_STARTUP_T 1200 // 1sec

class rx8025 : public i2c_rtc {
public:
  rx8025(){
    _POR_wait = DEFAULT_STARTUP_T; // at least 1sec;
    _addr = RX8025_ADDR;
    _t_buf = RX8025_T_BUF;
  }
  rx8025(uint16_t POR_wait){
    _POR_wait = POR_wait;
    _addr = RX8025_ADDR;
    _t_buf = RX8025_T_BUF;
  }
  rx8025(uint16_t POR_wait, uint32_t I2C_timeout){
    _POR_wait = POR_wait;
    _timeout = I2C_timeout;
    _addr = RX8025_ADDR;
    _t_buf = RX8025_T_BUF;
  }

private:
  bool _writeDateTime(rtc_tm *d);
  bool _readDateTime(rtc_tm *d);
  bool _checkValid();
  bool _init();
};

#endif // ifndef __RX8025_H__

// end of rx8025.h
