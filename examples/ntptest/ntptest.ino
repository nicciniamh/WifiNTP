#include <Time.h>
#include <TimeLib.h>
#include <WifiNTP.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>

#define PDT (-1*(7*3600)) // TZAdjust for Pacific Daylight Time

WifiNTP ntp;

char ssid[] = "foo";     //  your network SSID (name) 
char pass[] = "bar";    // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int clockInit = 0;


void setup() {
  Serial.begin(115200);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    while(true);  // don't continue
  }   
  // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  
  status = WiFi.begin(ssid, pass);
  WiFi.onEvent(WiFiEvent);
}
void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {
    case WIFI_EVENT_STAMODE_GOT_IP:
      ntp.begin(PDT);
      clockInit = 1;
      break;
    case WIFI_EVENT_STAMODE_DISCONNECTED:
      break;
  }
}

void loop() {
  time_t ut;
  if((ut = ntp.time()) <= 0xFFFFFFFE && clockInit == 1) {
    setTime(ut);
    clockInit = 2;
  }
  Serial.printf("ClkInit: %d %02d:%02d:%02d\n",clockInit,hour(),minute(),second());
  delay(1000);
}


