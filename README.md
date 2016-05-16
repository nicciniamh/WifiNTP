
This is my NTP library for Arduino. Please note the examples used are written 
for a WeMos D1 which uses an ESP8266 for the processor. YMMV. 

General usage is: 

WifiNTP ntp;
ntp.begin();
ntp.time();

This will, if you're connected to wifi, get UTC from time server(s).

Methods: 
	ntp.begin()	- Basic init of functions, use default servers.
	
	ntp.begin(long tzadjust) - Initialize functions and set the timezone adjustment, in seconds. Use default servers.
	
	ntp.begin(char ** ntpServers) - Init functions with null terminated array of pointers to time
	
	servers in dotted quad notation. A max of 4 will be used in round robin.
	
	ntp.time(tzadjust) - Return time, adjusting for timezone seconds. Use -1 or void for no adjustment. 
	
	
