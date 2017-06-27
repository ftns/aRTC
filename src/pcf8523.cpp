//$Id: pcf8523.cpp,v 1.4 2017/06/25 13:45:31 akihiro Exp akihiro $
// pcf8523.cpp

#include "pcf8523.h"

bool pcf8523::_init(){
  // Notice:
  // Section 8.5 "Power management functions " of the pcf8523 Datasheet 
  // is ambiguous.
  //
  // If there is no battery, Control_3 reg. should be 0x20/0x30?
  // and V_BAT must be equal to V_DD?
  //
  // However, if V_BAT & V_DD are connected, there seems to be  no 
  // problem with battery switch-over mode.

  return _writeBytes
    ( true, // send stopBit
      { PCF8523_REG_Control3, // write from addr 3/reg. SEC
	  0, // set battery switch-over mode
	  // initialize date, time, offset, alarm
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


// end of pcf8523.cpp
//
