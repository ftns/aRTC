//$Id: ds1307.h,v 1.5 2017/06/25 13:45:31 akihiro Exp akihiro $
// ds1307.h

#ifndef __DS1307_H__
#define __DS1307_H__

#include <Wire.h>
#include "i2c_rtc.h"
#include <time.h>

#define DS1307_ADDR 0x68
#define DS1307_T_BUF 5 // 4.7 micro sec

class ds1307 : public i2c_rtc {
public:
  ds1307(){
    _POR_wait = 0;
    _addr = DS1307_ADDR;
    _t_buf = DS1307_T_BUF;
  };
  ds1307(uint16_t POR_wait){
    _POR_wait = POR_wait;
    _addr = DS1307_ADDR;
    _t_buf = DS1307_T_BUF;
  };
  ds1307(uint16_t POR_wait, uint32_t I2C_timeout){
    _timeout = I2C_timeout;
    _POR_wait = POR_wait;
    _addr = DS1307_ADDR;
    _t_buf = DS1307_T_BUF;
  };
  bool isRunning();
  bool halt();
  bool restart();
  
private:
  bool _init();
  bool _checkValid();
  bool _setCH(bool ch);
  bool _writeDateTime(rtc_tm *d);
  bool _readDateTime(rtc_tm *d);
};

#endif // ifndef __DS1307_H__

// end of ds1307.h
