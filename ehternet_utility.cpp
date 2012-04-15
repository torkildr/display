#include "ethernet_utility.h"

byte mac[] = {  0x90, 0xa2, 0xda, 0x03, 0x00, 0x63 };

EthernetClient client;

void setupEthernet()
{
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // TODO: Handle failure gracefully, try again later?
  }
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void sendData()
{
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect("kaylee.markild.no", 80))
  
  {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.println("GET /crap/test.txt HTTP/1.1");
    client.println("GET /crap/test.txt HTTP/1.1");
    Serial.println("Host: kaylee.markild.no");
    client.println("Host: kaylee.markild.no");
    Serial.println("User-Agent: ArduinoUno/r3 Ethernet Shield");
    client.println("User-Agent: ArduinoUno/r3 Ethernet Shield");
    Serial.println("Accept: */*");
    client.println("Accept: */*");    
    client.println();
  }
  else
  {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void getData(char *dst)
{
  Serial.println("Trying to grab data from server:");
  
  // from the server, read them and print them:
  int i = 0;
  
  while (!client.available());

  while (client.available()) {
    char c = client.read();
    Serial.print(c);
    
    if (i < 10)
    {
      dst[i] = c;
      //dst[i+1] = 0;
    }

    i++;
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }
}

