/*-----------------------------------------------------------------------------
WifiNTP library v0.2 Copyright (C) Nicole Stevens 
  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <ESP8266WiFi.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <time.h>
#include <WiFiUdp.h>
#include "WifiNTP.h"

// #define DEBUG(x) (Serial.println(x))
#define DEBUG(x)

WifiNTP::WifiNTP(void) { 
  int i;
  for(i = 0; i < ntpPoolMax; i++) 
    ntpPool[i] = NULL;
  begun = false;
}
void WifiNTP::begin(long _tzadjust) {
	begin(NULL,_tzadjust);
}
void WifiNTP::begin(void) {
  begin(NULL,-1);
}
void WifiNTP::begin(char **ntpServers) {
  begin(ntpServers,-1);
}
void WifiNTP::begin(char **ntpServers, long _tzadjust) {
	int i;
	if(tzadjust != -1)
		tzadjust = _tzadjust;
	else
		tzadjust = 0;
	curServer = 0;
	if(ntpServers != NULL) {
	  for(i = 0; ntpServers[i] != 0 && i < ntpPoolMax; i++)
		ntpPool[i] = ntpServers[i];
	} else {
		ntpPool[0] = "192.5.41.40"; // tick.usno.navy.mil;
		ntpPool[1] = "192.5.41.41"; // tock.usno.navy.mil;
	}
	begun = 1;
	sendPacket();
}
void WifiNTP::sendPacket() {
	IPAddress ip; 
	if(!begun) {
		DEBUG("NOBEGIN");
		epochtime = NOBEGIN;
		return;
	}
	if(ntpPool[0] == NULL) {
		DEBUG("NOSRV");
		epochtime = NOSRV;
		return;
	}
	epochtime = NOTIME;
	if(ntpPool[curServer] == NULL) {
	  curServer = 0;
	}
	gotTime = false;
	ip.fromString(ntpPool[curServer++]);
  
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;
	Udp.begin(ntpClntPort);
	Udp.beginPacket(ip, ntpSrvPort); //NTP requests are to port 123
	Udp.write(packetBuffer, NTP_PACKET_SIZE);
	Udp.endPacket();
}


time_t WifiNTP::getNTP() { 
  if(!begun || ntpPool[0] == 0)
    return epochtime;
  if(gotTime) {
    gotTime = false;
    return epochtime;
  }
  if (Udp.parsePacket()) {
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    epochtime = secsSince1900 - seventyYears;
    gotTime = true;
    return epochtime;
  }
  return epochtime = NOTIME;
}
time_t WifiNTP::time(void) {
	return time(-1);
}
time_t WifiNTP::time(long _tzadjust = -1) {
	time_t epoch;
	if(epochtime >= 0xFFFFFFF0 && epochtime != NOTIME)
		return epochtime;
	if(epochtime == 0 || epochtime == NOTIME) {
		if((epoch = getNTP()) == NOTIME)
			return epoch;
	} else
		epoch = epochtime;
	if(_tzadjust == -1) 
		return epoch + tzadjust;
	return epoch + _tzadjust;
}
