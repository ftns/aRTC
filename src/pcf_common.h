// 
// aRTC/src/pcf_common.h

#ifndef __PCF_COMMON_H__
#define __PCF_COMMON_H__

#include "i2c_rtc.h"

#define PCF_COMMON_REG_SEC 0x03

class pcf_common : public i2c_rtc {
protected:
  bool _writeDateTime(rtc_tm *d);
  bool _readDateTime(rtc_tm *d);
  bool _checkValid();
};

#endif // ifndef __PCF_COMMON_H__

// end of pcf_common.h
