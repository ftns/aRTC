// simple example

#include <Arduino.h>

// Uncomment apropriate header.
//#include <rtc8564.h>
//#include <rx8025.h>
//#include <ds1307.h>
//#include <pcf2129.h>
#include <pcf8523.h>

// Uncomment apropriate object.
//rtc8564 rtc;
//rx8025 rtc;
//ds1307 rtc;
//pcf2129 rtc;
pcf8523 rtc;

void setup() {
  Serial.begin(115200);

  Wire.begin();

  // if you want use wire object except default Wire,
  // insert below one line BEFORE rtc.startRTC.
  // rtc.setWire(otherWire);

  rtc.startRTC();

  if (!rtc.isValid()) {
    // RTC value is not valid, so you must set date and time.
    // like:
    //   rtc.setDateTime(YEAR, MONTH, DAY, HOUR, MIN, SEC);
    // or
    //   struct rtc_tm *t;
    //   t->rtc_sec = SEC;
    //   t->rtc_min = MIN;
    //   t->rtc_hour = HOUR;
    //   t-> rtc_mday = DAY;
    //   t->rtc_month = MONTH;
    //   t->rtc_year = YEAR;
    //   rtc.setDateTime(t);
    // or
    //   time_t epoch = NTP.getEpochTime();
    //   rtc.setEpoch(epoch);
  }
}
  
const char *DayOfW [] ={"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};

void loop() {
  struct rtc_tm d;
  char buf[32];

  rtc.getDateTime(&d);
  // print date
  Serial.printf("%d/%d/%d %s ",
		d.rtc_year, d.rtc_month, d.rtc_mday, DayOfW[d.rtc_wday]);
  // print time
  Serial.printf("%02d:%02d:%02d\n",
		d.rtc_hour, d.rtc_min, d.rtc_sec);

  delay(10); // sleep 10ms
}

// end of simple.ino
