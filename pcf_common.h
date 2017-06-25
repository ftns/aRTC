// $Id: pcf_common.h,v 1.1 2017/06/25 13:45:31 akihiro Exp akihiro $
// pcf_common.h

#ifndef __PCF_COMMON_H__
#define __PCF_COMMON_H__

#include "i2c_rtc.h"

#define PCF_COMMON_REG_SEC 0x03

#define DEFAULT_STARUP_T 2000; // 2000m sec

class pcf_common : public i2c_rtc {
protected:
  int _POR_wait = DEFAULT_STARUP_T; // default: 2000 msec
  bool _writeDateTime(rtc_tm *d);
  bool _readDateTime(rtc_tm *d);
  bool _checkValid();
};

#endif // ifndef __PCF_COMMON_H__

// end of pcf_common.h
