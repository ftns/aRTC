// 
// ESP8266.ino
//   working example on ESP8266(ESP-WROOM-02)
//   This example is used for debug.

// WiFi
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

const char *ssid = "WiFi_SSID";
const char *password = "WiFi_PWD";

// NTP
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.jst.mfeed.ad.jp", // NTP server
         32400,           // JST=UTC+9: 9h=9(h)*60(min)*60(sec)=32400(sec)
         86400000);  // 24h=24(h)*60(min)*60(sec)*1000(msec)=86400000(sec)

// RTC
//#include <rtc8564.h>
//#include <ds1307.h>
//#include <rx8025.h>
//#include <pcf2129.h>
#include <pcf8523.h>

// declare rtc object
//rtc8564 rtc;
//ds1307 rtc;
//rx8025 rtc;
//pcf2129 rtc;
pcf8523 rtc;

#define FORCE_SET_NTP_TIME 1

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true); // for WiFi

  Serial.printf("===\nstart setup()\n===\n");
  Wire.begin(4, 5);
  Wire.setClock(300000); // 300kHz

  if (! rtc.startRTC()){
    Serial.printf("RTC init returns false. err: %x\n", rtc.rtc_errno());
    delay(10000);
  }

  if (rtc.isValid()) {
    Serial.printf("rtc.isValid returns true.\n");
  } else {
    Serial.printf("rtc.isValid returns false.\n");
    Serial.printf("Invoke WiFi.begin()\n");
  }

  if ( (!rtc.isValid())|| FORCE_SET_NTP_TIME) {
    int w;
    WiFi.begin(ssid, password);
    while ((w = WiFi.status()) != WL_CONNECTED) {
      Serial.printf("%d", w);
      // delay(500);
    }   
    Serial.printf("\nInvoke NTP\n");
    timeClient.begin();
    timeClient.update();
    if (! rtc.setEpoch(timeClient.getEpochTime())){
      Serial.printf("rtc.setEpoch returns false. err: %02x\n", rtc.rtc_errno());
      // delay(100000);
    }
  }
  Serial.printf("===\nend setup()\n===\n");
}

const char *DayOfW [] ={"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};

void loop() {
  struct rtc_tm d;
  
  if (! rtc.isValid()){
    Serial.printf("Warning: rtc.isValid() returns false.\n");
  }

  if (!rtc.getDateTime(&d)) {
    Serial.printf("Warning: rtc.getDateTime returns false.\n");
  }
  
  Serial.printf("%d/%d/%d %s\n\n",
                d.rtc_year, d.rtc_month, d.rtc_mday, DayOfW[d.rtc_wday]);
  delay(500); // sleep 500ms
}

// end of ESP8266.ino
//
