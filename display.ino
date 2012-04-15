#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <util.h>
#include <SPI.h>
#include "display.h"
#include "display_utility.h"
#include "Time.h"
#include "ntp_time.h"
#include "ethernet_utility.h"

///////////////////////////////
// Setup functions
///////////////////////////////

// Global variables for text positioning
byte scroll_enabled;
char* display_text;
int display_length;
int display_column;

void setupDisplayTimer() {
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  //OCR1A = 6000;            // compare match register 16MHz/256/2Hz
  OCR1A = 32000;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  
  interrupts();             // enable all interrupts
}

///////////////////////////////
// end helper functions
///////////////////////////////

///////////////////////////////////////////////////////////
// Main stuff!
///////////////////////////////////////////////////////////

char * textBuffer;
byte dirty;
boolean loopEnabled;

void refreshEthernet()
{
  // Time for ethernet stuff
  textBuffer = (char *) malloc(100);
  strcpy(textBuffer, "Connecting...");

  setupEthernet();
}

// set up
void setup() {
  // start the serial library:
  Serial.begin(9600);

  // All display stuff
  
  setupDisplay();
  clearDisplay();
  
  display_column = 5;
  scroll_enabled = 0;
  dirty = 0;
  
  setupDisplayTimer();

  refreshEthernet();
  setupNTP();
  
  loopEnabled = true;
}

void setupScrollingText(char* text)
{
  display_text = text;
  display_column = getXmax();

  clearDisplay();
}

// Display timer, refreshes text, potentially scrolls as well
// TODO: Use callback to scroll text?
ISR(TIMER1_COMPA_vect)
{
  noInterrupts();

  syncDisplay();
  clearDisplay();

  byte skipped = printText(display_column, textBuffer);

  if (scroll_enabled)
  {
    if (skipped) {
      ++display_text;
      --display_length;
      display_column += skipped;
    }
  
    --display_column;
    if (!display_length)
    {
      setupScrollingText(textBuffer);
    }
  }

  interrupts();
}

time_t prevDisplay = 0;
byte brightness = 15;

extern void *__brkval;

int showFreeMem()
{
  int free_memory;
  free_memory = ((int)&free_memory) - ((int)__brkval);
  
  Serial.print("Free mem: ");
  Serial.println(free_memory);
}

void loop() {
  if(now() != prevDisplay && loopEnabled) // each second
  {
    prevDisplay = now();
    
    char monthName[4];
    strcpy(monthName, monthShortStr(month()));
    
    // if not date is after 2000, we're calling bullshit
    if (prevDisplay > 946684800UL)
      sprintf(textBuffer, "%s, %s %d, %02d:%02d ", dayStr(weekday()), monthName, day(), hour(), minute());
    else
      sprintf(textBuffer, "Connecting...");
    
    Serial.println(textBuffer);
    
    // 23 -> 08, dimmed down
    if (hour() >= 23 || hour() < 8) {
      if (brightness != 4) {
        brightness = 4;
        setBrightness(brightness);
        Serial.println("Setting brightness to low");
      }
    } else {
      if (brightness != 15) {
        brightness = 15;
        setBrightness(brightness);
        Serial.println("Setting brightness to high");
      }
    }


    showFreeMem();
  }
}

