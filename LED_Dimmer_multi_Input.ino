/*
LED_Dimmer by New American Public Art is licensed under a 
Creative Commons Attribution-ShareAlike 4.0 International License.
*/

#include "LPD8806.h"
#include "SPI.h"
#define FADE_TABLE_SIZE (sizeof(LED_Fade_Table))
#define FADE_CYCLE    5000      /*fade cycle in milliseconds*/
#define FADE_UPDATE    (FADE_CYCLE / FADE_TABLE_SIZE)

int dataPin = 13;
int clockPin = 12;
int motion1 = 8;
int motion2 = 9;
int motion3 = 10;
int motion4 = 11;

// The LED strips are 32 LEDs per meter but you can extend/cut the strip
LPD8806 strip = LPD8806(160, dataPin, clockPin);

//TABLE BASED ON MY MATH CALCS OF exp(sin(x)), shifted so bottom is 15   
// These two tables should have the same amount of variables
uint8_t LED_Fade_Table[]  = {
5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 15, 15, 16, 16, 17, 17, 18, 19, 20, 22, 23,
25, 27, 29, 31, 33, 36, 39, 43, 46, 50, 54, 59, 63, 69, 74,
80, 86, 93, 100, 107, 114, 122, 130, 139, 147, 156, 164, 173, 
182, 190, 198, 206, 214, 221, 227, 233, 238, 242, 246, 248,
250, 250};

uint8_t LED_Fade_Table_2[]  = {
250, 250, 249, 246, 243, 239, 235, 229, 223, 216, 209,
201, 193, 184, 176, 167, 158, 150, 141, 133, 125, 117, 109,
102, 95, 88, 82, 76, 70, 65, 60, 55, 51, 47, 44, 40, 37, 34,
32, 29, 27, 25, 24, 22, 21, 20, 19, 18, 17, 16, 16, 16, 15,
15, 15, 15, 14, 13, 11, 10, 9, 8, 7, 6, 5};

uint8_t curFadeIndex = 0;
uint8_t fadeIndex = 0;
uint8_t fadeRed;
uint8_t fadeGrn;
uint8_t fadeBlu;

//--------------------------------------------------------------------!  SETUP  !-----------///////

void setup() {
  
  int i;
  pinMode(motion1, INPUT);
  pinMode(motion2, INPUT);
  pinMode(motion3, INPUT);
  pinMode(motion4, INPUT);
  Serial.begin(9600);   
  strip.begin();
  // Update the strip, to start they are all 'blue'
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0, 0, 255);
    }
  strip.show();
}

//--------------------------------------------------------------------!  LOOP  !-------------///////

void loop() {
  
  if(digitalRead(motion1) || digitalRead(motion2) || digitalRead(motion3) || digitalRead(motion4))
  { 
    Serial.println("fadeup");
    FadeUp(LED_Fade_Table, LED_Fade_Table_2, FADE_TABLE_SIZE, FADE_UPDATE, 127, 90, 0, 0, 0, 127);
   } else {
    Serial.println("fadedown"); 
    FadeDown(LED_Fade_Table, LED_Fade_Table_2, FADE_TABLE_SIZE, FADE_UPDATE, 127, 90, 0, 0, 0, 127);
  }
}

//--------------------------------------------------------------------!  FUNCTIONS  !--------/////// 


void FadeUp(uint8_t* fadeTable, uint8_t* fadeTable2, uint8_t fadeTableSize, uint16_t updatePeriod, uint16_t r, uint16_t g, uint16_t b, uint16_t r2, uint16_t g2, uint16_t b2)
{
  int i;
  for (fadeIndex = curFadeIndex; fadeIndex < fadeTableSize; fadeIndex++) {
    //Serial.println(fadeIndex);
    for (i=0; i < strip.numPixels(); i+=2) {
      fadeRed = (r * fadeTable[fadeIndex]) / 256;
      fadeGrn = (g * fadeTable[fadeIndex]) / 256;
      fadeBlu = (b * fadeTable[fadeIndex]) / 256;
      strip.setPixelColor(i, fadeRed, fadeGrn, fadeBlu);
    }
    for (i=1; i < strip.numPixels(); i+=2) {
      fadeRed = (r2 * fadeTable2[fadeIndex]) / 256;
      fadeGrn = (g2 * fadeTable2[fadeIndex]) / 256;
      fadeBlu = (b2 * fadeTable2[fadeIndex]) / 256;
      strip.setPixelColor(i, fadeRed, fadeGrn, fadeBlu);
    }
    strip.show();   // write all the pixels out
    curFadeIndex++;
    if(curFadeIndex > fadeTableSize){
      curFadeIndex = fadeTableSize;
    }
    delay(updatePeriod);
    return;
  }
}

void FadeDown(uint8_t* fadeTable, uint8_t* fadeTable2, uint8_t fadeTableSize, uint16_t updatePeriod, uint16_t r, uint16_t g, uint16_t b, uint16_t r2, uint16_t g2, uint16_t b2)
{
  int i;
  for (fadeIndex = curFadeIndex; fadeIndex > 0; fadeIndex--) {
    for (i=0; i < strip.numPixels(); i++) {
      fadeRed = (r * fadeTable[fadeIndex]) / 256;
      fadeGrn = (g * fadeTable[fadeIndex]) / 256;
      fadeBlu = (b * fadeTable[fadeIndex]) / 256;
      strip.setPixelColor(i, fadeRed, fadeGrn, fadeBlu);
    }
    for (i=1; i < strip.numPixels(); i+=2) {
      fadeRed = (r2 * fadeTable2[fadeIndex]) / 256;
      fadeGrn = (g2 * fadeTable2[fadeIndex]) / 256;
      fadeBlu = (b2 * fadeTable2[fadeIndex]) / 256;
      strip.setPixelColor(i, fadeRed, fadeGrn, fadeBlu);
    }
    strip.show();   // write all the pixels out
    curFadeIndex--;
    if(curFadeIndex < 0){
      curFadeIndex = 0;
    }
    delay(updatePeriod);
    return;
  } 
}





