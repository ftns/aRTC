//$Id: pcf2129.h,v 1.3 2017/06/25 13:45:31 akihiro Exp akihiro $
// pcf2129.h

#ifndef __PCF2129_H__
#define __PCF2129_H__

#include "pcf_common.h"

#define PCF2129_ADDR 0x51
#define PCF2129_T_BUF 5 // 4.7 micro sec(min)
#define PCF2129_REG_SEC 0x03

class pcf2129 : public pcf_common {
public:
  pcf2129(){
    // _POR_wait = DEFAULT_STARUP_T;
    _addr = PCF2129_ADDR;
    _t_buf = PCF2129_T_BUF;
  }
  pcf2129(uint16_t POR_wait){
    _POR_wait = POR_wait; // wakeup time on power on reset
    _addr = PCF2129_ADDR;
    _t_buf = PCF2129_T_BUF;
  }
  pcf2129(uint16_t POR_wait, uint32_t I2C_timeout){
    _POR_wait = POR_wait;
    _timeout = I2C_timeout;
    _addr = PCF2129_ADDR;
    _t_buf = PCF2129_T_BUF;
  }

private:
  bool _init();
};

#endif // ifndef __PCF2129_H__

// end of pcf2129.h
