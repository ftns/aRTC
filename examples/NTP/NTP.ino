// NTP.ino
// set date&time from NTP

#include <Arduino.h>

// WiFi
#include <WiFiUdp.h>

// NTP
#include <NTPClient.h>

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

const char *ssid = "your SSID";
const char *password = "your PWD";

// Wifi & NTP setup
WiFiUDP ntpUDP;
NTPClient timeClient
( ntpUDP,
  "your-ntp-server", // NTP server
  32400, // JST=UTC+9: 9h=9(h)*60(min)*60(sec)=32400(sec)
  86400000);// 24h=24(h)*60(min)*60(sec)*1000(msec)=86400000(sec)


void setup() {
  Serial.begin(115200);

  Wire.begin();

  // if you want use wire object except default Wire,
  // insert below one line BEFORE rtc.startRTC.
  // rtc.setWire(otherWire);

  rtc.startRTC();

  if (!rtc.isValid()) {
    int w;

    // enable WiFi
    WiFi.begin(ssid, password);
    while ((w = WiFi.status()) != WL_CONNECTED) {
      Serial.printf("%d", w);
      delay(500);
    }   
    Serial.printf("\n");

    // enable NTP client
    timeClient.begin();
    timeClient.update();
    
    // Set date & time from NTP server.
    rtc.setEpoch(timeClient.getEpochTime());
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

  delay(100); // sleep 100ms
}

// end of NTP.ino
