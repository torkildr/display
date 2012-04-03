#include <Arduino.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <util.h>

#include <SPI.h>

#ifndef ETHERNET_UTILITY_H
#define ETHERNET_UTILITY_H

void setupEthernet();

void getData(char *dst);

void sendData();

#endif



