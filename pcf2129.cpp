//$Id: pcf2129.cpp,v 1.3 2017/06/25 13:45:31 akihiro Exp akihiro $
// pcf2129.cpp

#include "pcf2129.h"

bool pcf2129::_init(){
  if (! _writeBytes(true, {0x0F, 0x07})) return false;
  // Reg.0F: Temp. period=4min, OTPR=0, CLKOUT off

  if (! _writeBytes(true, {0x0F, 0x27})) return false;
  // Reg.0F: Temp. period=4min, OTPR=1, CLKOUT off

  if (! _writeBytes(true, {0x00, 0x00})) return false;
  // Reg.0: Power-On Reset Override disabled.  

  return _writeBytes
    (
     true, 
     {0x03, // write from registor 03
	 // set to 0:00:00
	 0x00, // Reg.3: seconds by BCD & clear OSF
	 0x00, // Reg.4: minutes by BCD
	 0x00, // Reg.5: hours by BCD in 24h mode
	 // set to 2017/1/1 (Sun.)
	 0x01, // Reg.6: day of month by BCD
	 0x00, // Reg.7: day of week: Sunday
	 0x01, // Reg.8: month by BCD
	 0x17}); // Reg.9: Years by BCD  
}


// end of pcf2129.cpp
//
