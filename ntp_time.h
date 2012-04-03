#include "Time.h"
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Dns.h>

#ifndef _NTP_TIME_H
#define _NTP_TIME_H
 
void setupNTP();

unsigned long getNtpTime();

unsigned long sendNTPpacket(IPAddress& address);

#endif
