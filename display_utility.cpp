#include "display_utility.h"
#include "font.h"

#include "ht1632_cmd.h"
#include "MatrixDisplay.h"

// Prepare display boundaries
uint8_t X_MAX = 0;
uint8_t Y_MAX = 0;

// Number of displays (1-4)
// Shared clock pin
// Shared data pin
// Build shadow
MatrixDisplay disp(DISPLAY_COUNT, PIN_CLOCK, PIN_DATA);

void setupDisplay() {
  // disp number, clk select pin, bool master
  if (DISPLAY_COUNT >= 1)
    disp.initDisplay(0, PIN_CS_DISP1, true);
  if (DISPLAY_COUNT >= 2)
    disp.initDisplay(1, PIN_CS_DISP2, false);
  if (DISPLAY_COUNT >= 3)
    disp.initDisplay(2, PIN_CS_DISP3, false);
  if (DISPLAY_COUNT >= 4)
    disp.initDisplay(3, PIN_CS_DISP4, false);
  
  // Fetch display bounds 
  X_MAX = disp.getDisplayCount() * disp.getDisplayWidth();
  Y_MAX = disp.getDisplayHeight();
  
  Serial.print("X_MAX: ");
  Serial.println(X_MAX);
  Serial.print("Y_MAX: ");
  Serial.println(Y_MAX);
}

uint8_t getXmax()
{
  return X_MAX;
}

void clearDisplay()
{
  disp.clear(false);
}

void syncDisplay()
{
  disp.syncDisplays();
}

// Output a single character to the display
byte printChar(int x, char c, byte *skipped){
  c = charLookup.indexOf(c);
  byte width = pgm_read_byte_near(&font_variable[c][0]);
  
  if (x + width + 1 == 0) {
    *skipped = 1;
    return width;
  }

  for (byte col = 0; col <= width; col++) {
     // dont write to the display buffer if the location is out of range
    if((x + col) >= 0 && (x + col) < X_MAX){
      // reads entire row of glyph, jams it into memory
      if (col != width)
        disp.setRow(x + col, pgm_read_byte_near(&font_variable[c][col+1]));
      else
        disp.setRow(x + col, 0);
    }
  }
  
  return width;
}

// Output a string to the display
byte printText(int x, char* c, int strLength) {
  byte skippedChars = 0;
  
  for(char i=0; i < strLength; i++) {
    byte skipped = 0;
    byte width = printChar(x, c[i], &skipped) + 1;
    
    if (skipped) {
      skippedChars = width;
    }

    x += width;
    if (x > X_MAX)
      return skippedChars;
  }
  
  return skippedChars;
}

int stringWidth(char* text, int strLength) {
  int x = 0;
  
  for(char i = 0; i < strLength; i++){
    byte c = charLookup.indexOf(text[i]);
    byte width = pgm_read_byte_near(&font_variable[c][0]);
    x += width;
  }
  
  return x;
}

// Output a string to the display
byte printText(int x, char* c) {
  return printText(x, c, strlen(c));
}

void printText(char* c) {
  disp.clear(true);

  // try to remove everything first, to remove "ghosting"
  //disp.syncDisplays(false);

  printText(0, c);
  disp.syncDisplays();
}

// text needs to be zero terminated
void scrollText(char* text, short slowdown) {
  int textLength = strlen(text);

  // needed?
  disp.clear(false);

  int Xpos = X_MAX;

  while (textLength) {
    byte skipped = printText(Xpos, text, textLength);
    
    if (skipped) {
      ++text;
      --textLength;
      Xpos += skipped;
    }

    disp.syncDisplays();
    delay(slowdown);
    
    --Xpos;
  }
}

void setBrightness(uint8_t pwmValue)
{
  for (int i=0; i < DISPLAY_COUNT; i++)
  {
    disp.setBrightness(i, pwmValue);
  }
}

