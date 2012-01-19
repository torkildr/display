#include <Arduino.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#ifndef UTILITY_H
#define UTILITY_H

// Matrix display values
#define DISPLAY_COUNT 4

#define PIN_CLOCK 3
#define PIN_DATA 9

#define PIN_CS_DISP1 5
#define PIN_CS_DISP2 6
#define PIN_CS_DISP3 7
#define PIN_CS_DISP4 8

byte printChar(int x, char c, byte *skipped);

void printText(char* c);

byte printText(int x, char* c);

byte printText(int x, char* c, int strLength);

int stringWidth(char* text, int strLength);

void scrollText(char* text, short slowdown);

void setupDisplay();

#endif
