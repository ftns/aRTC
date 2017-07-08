//
// aRTC/src/pcf8523.h

#ifndef __PCF8523_H__
#define __PCF8523_H__

#include "pcf_common.h"

#define PCF8523_ADDR 0x68
#define PCF8523_T_BUF 5 // 4.7 micro sec(min)
#define PCF8523_POR_T 2000 // 2000msec

// register adress
#define PCF8523_REG_Control1 0x00
#define PCF8523_REG_Control2 0x01
#define PCF8523_REG_Control3 0x02
#define PCF8523_REG_SEC 0x03

class pcf8523 : public pcf_common {
public:
  pcf8523(){
    setPORwait(PCF8523_POR_T);
    setADDR(PCF8523_ADDR);
    setT_BUF(PCF8523_T_BUF);
  }
  pcf8523(uint16_t POR_wait){
    pcf8523();
    setPORwait(POR_wait);
  }
  pcf8523(uint16_t POR_wait, uint32_t I2C_timeout){
    pcf8523();
    setPORwait(POR_wait);
    setTimeout(I2C_timeout);
  }

private:
  bool _init();
};

#endif // ifndef __PCF8523_H__

// end of pcf8523.h
