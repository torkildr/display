#include "ntp_time.h"

#define LOCAL_PORT 30888

#define TIMEZONE_OFFSET 2L // set this to the offset in hours to your local time;

const int NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setupNTP()
{
  Serial.println("Waiting for NTP sync");
  
  Udp.begin(LOCAL_PORT);

  setSyncProvider(getNtpTime);
  while(timeStatus() == timeNotSet); // wait until the time is set by the sync provider
}

byte ntpTimeout;

unsigned long getNtpTime()
{
  sendNTPpacket(); // send an NTP packet to a time server

    // wait to see if a reply is available
  delay(1000);
  
  if (Udp.parsePacket()) {  
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  

    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;  
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);              

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;    
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;  
    // print Unix time:
    Serial.println(epoch);                              

    ntpTimeout = 10;
    return epoch + (TIMEZONE_OFFSET * SECS_PER_HOUR);
  } else {
    if (ntpTimeout > 0) {
      ntpTimeout--;
    } else {
      // give a "real" time, but one that we can sniff out
      return 1;
    }
    

    Serial.println("Missed package");
  }
  
  return 0;
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket()
{
  DNSClient dns;
  IPAddress address;
  
  dns.begin(Ethernet.dnsServerIP());
  // TODO: make address configurable
  dns.getHostByName("beate.lan", address);

  Serial.print("Sending NTP packet to : ");
  Serial.println(address);
  
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:         
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

