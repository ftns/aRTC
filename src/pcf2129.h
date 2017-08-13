//
// aRTC/src/pcf2129.h v1.0
//
// Original source is written by Akihiro SHIMIZU.
// This source is published under license of LGPL v.3
//

#ifndef __PCF2129_H__
#define __PCF2129_H__

#include "pcf_common.h"

#define PCF2129_ADDR 0x51
#define PCF2129_T_BUF 5 // 4.7 micro sec(min)
#define PCF2129_REG_SEC 0x03

#define PCF2129_POR_T 2000 // 2000m sec

class pcf2129 : public pcf_common {
public:
  pcf2129(){
    setADDR(PCF2129_ADDR);
    setT_BUF(PCF2129_T_BUF);
    setPORwait(PCF2129_POR_T);
  }
private:
  bool _init();
};

#endif // ifndef __PCF2129_H__

// end of pcf2129.h
