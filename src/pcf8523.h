//$Id: pcf8523.h,v 1.3 2017/06/25 13:45:31 akihiro Exp akihiro $
// pcf8523.h

#ifndef __PCF8523_H__
#define __PCF8523_H__

#include "pcf_common.h"

#define PCF8523_ADDR 0x68
#define PCF8523_T_BUF 5 // 4.7 micro sec(min)

// register adress
#define PCF8523_REG_Control1 0x00
#define PCF8523_REG_Control2 0x01
#define PCF8523_REG_Control3 0x02
#define PCF8523_REG_SEC 0x03

class pcf8523 : public pcf_common {
public:
  pcf8523(){
    // _POR_wait = DEFAULT_STARUP_T; -> pcf_common
    _addr = PCF8523_ADDR;
    _t_buf = PCF8523_T_BUF;
  }
  pcf8523(uint16_t POR_wait){
    _POR_wait = POR_wait; // wakeup time on power on reset
    _addr = PCF8523_ADDR;
    _t_buf = PCF8523_T_BUF;
  }
  pcf8523(uint16_t POR_wait, uint32_t I2C_timeout){
    _POR_wait = POR_wait;
    _timeout = I2C_timeout;
    _addr = PCF8523_ADDR;
    _t_buf = PCF8523_T_BUF;
  }

private:
  bool _init();
};

#endif // ifndef __PCF8523_H__

// end of pcf8523.h
