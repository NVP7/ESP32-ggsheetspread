#include <WiFi.h>
#include <HTTPClient.h>
#include "RTClib.h"
#include <SPI.h>

RTC_DS3231 rtc;

const char * ssid = "";
const char * password = "";
String GOOGLE_SCRIPT_ID = "";
//---------------------------------------------------------------------

const int sendInterval = 2000;

void write_to_google_sheet(String params) {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
   //Serial.print(url);
    Serial.println("Postring Time data to Google Sheet");

    http.begin(url.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();  
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);     
    }
    //---------------------------------------------------------------------
    http.end();
}

 void setup() {
  Serial.begin(115200);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(2023, 7, 11,9,54,0));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  //--------------------------------------------
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");
}

void loop() {
  DateTime now = rtc.now();
  char Buffyear[5];
  char Buffmonth[5];
  char Buffday[5];
  char Buffhour[5];
  char Buffminute[5];
  char Buffsecond[5];
  for (unsigned long start = millis(); millis() - start < 5000;)
  {
    sprintf(Buffyear,"%02d", now.year());
    sprintf(Buffmonth,"%02d", now.month());
    sprintf(Buffday,"%02d", now.day());
    sprintf(Buffhour,"%02d", now.hour());
    sprintf(Buffminute,"%02d", now.minute());
    sprintf(Buffsecond,"%02d", now.second());

    String Year(Buffyear);
    String Month(Buffmonth);
    String Day(Buffday);
    String Hour(Buffhour);
    String Minute(Buffminute);
    String Second(Buffsecond);

    String param;
    param  = "Year=" + String(Year);
    param += "&Month="  + String(Month);
    param += "&Day=" + String(Day);
    param += "&Hour=" + String(Hour);
    param += "&Minute=" + String(Minute);
    param += "&Second=" + String(Second);

    Serial.println(param);
    write_to_google_sheet(param);
  }
  }