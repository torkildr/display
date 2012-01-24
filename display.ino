#include "display.h"
#include "display_utility.h"

///////////////////////////////
// Setup functions
///////////////////////////////

// Buffer pointer, more on this later
char* text;

void setupPhotocell() {
  pinMode(PIN_PHOTOCELL, INPUT);
  pinMode(PIN_LED, OUTPUT);
  
  text = (char *) malloc(17 * sizeof(char));
}

void setupTimer() {
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 6000;            // compare match register 16MHz/256/2Hz
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

// set up
void setup() {
  // start the serial library:
  Serial.begin(9600);

  // Setup display
  // number of displays
  setupDisplay();
  
  setupTimer();
  
  setupPhotocell();
}

int photocell;

char* display_text;
int display_length;
int display_column;

void setupScrollingText(char* text)
{
  display_length = strlen(text);
  display_text = text;
  display_column = getXmax();

  clearDisplay();
}

char* defaultText = "Hello, World!!";

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  syncDisplay();

  byte skipped = printText(display_column, display_text, display_length);
  
  if (skipped) {
    ++display_text;
    --display_length;
    display_column += skipped;
  }

  --display_column;

  if (!display_length)
  {
    setupScrollingText(defaultText);
  }
}

void loop() {
  //scrollText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\xe6\xf8\xe5\xc6\xd8\xc5", 50);
}

