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
#include <WiFiUdp.h>
#if !defined(___WIFINTP_H)
#define  ___WIFINTP_H
#define NOTIME	((time_t)0xFFFFFFFF)
#define NOSRV 	((time_t)0xFFFFFFFE)
#define NOBEGIN ((time_t)0xFFFFFFFD)

class WifiNTP {
	private:
		// NTP time stamp is in the first 48 bytes of the message
		static const int NTP_PACKET_SIZE = 48;
		static const int ntpPoolMax = 4;
		static const int ntpSrvPort = 123;
		static const int ntpClntPort = 61613;
	time_t epochtime;
		bool begun;
		long tzadjust;
		byte packetBuffer[ NTP_PACKET_SIZE]; 
		int curServer;
		char * ntpPool[4];
		WiFiUDP Udp;
		bool gotTime;
		void sendPacket();
	public:
		WifiNTP();
		void begin(char **ntpServers, time_t now);
		void begin(char **ntpServers);
		void begin(long tzadjust);
		void begin(void);
		void begin(char **ntpServers, time_t now, long _tzadjust);
		time_t getNTP(void);
		time_t time(long tzadjust);
};
	
#endif
