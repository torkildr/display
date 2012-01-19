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

  OCR1A = 31250;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  digitalWrite(A0, digitalRead(A0) ^ 1);   // toggle LED pin
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

/*
void photocell_display() {
  photocell = analogRead(PIN_PHOTOCELL);

  snprintf(text, 17, "%4d", photocell);
  
  if (photocell > 512)
    digitalWrite(PIN_LED, HIGH);
  else
    digitalWrite(PIN_LED, LOW);
  
  for (int i=0; i < 4; i++) {
    disp.setBrightness(i, photocell / 64);
  }
  
  disp.clear();
    
  printText(0, text);
  printText(32, text);
  printText(64, text);
  printText(96, text);
  
  disp.syncDisplays();
  
  delay(150);
}
*/

void loop() {
  /*
  printText("Dette er en test");
  delay(5000);
  printText("Av litt mindre teit m\xe5te \xe5 vise..");
  delay(8000);
  printText("Tekst p\xe5!");
  delay(4000);
  */

  //scrollText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\xe6\xf8\xe5\xc6\xd8\xc5", 50);

  scrollText("Dette er en liten test: torkild@retvedt.no ;-)", 100);
}

