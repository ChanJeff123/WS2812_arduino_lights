#include <FastLED.h>
FASTLED_USING_NAMESPACE
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/TomThumb.h>
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif
#define DATA_PIN    3
const int KEY = 2;
const int analogInPin = A0;
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS   50
CRGB leds[NUM_LEDS];
int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

int sensorValue = 0;
int outputValue = 0;
#define FRAMES_PER_SECOND  100
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(10, 5, DATA_PIN,
                            //  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                            NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};
void setup() {
  pinMode(KEY, INPUT);
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  matrix.setFont(&TomThumb);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setTextColor(colors[0]);
}
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0;
uint8_t gHue = 0;

int x = matrix.width();
int pass = 0;

void loop()
{
  gPatterns[gCurrentPatternNumber]();
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  buttonState = digitalRead(KEY);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
    } else {
      Serial.println("off");
    }
    delay(50);
  }
  lastButtonState = buttonState;
  if (buttonPushCounter % 5 == 0) {
    RBOW();
  }
  else if (buttonPushCounter % 5 == 1) {
    matrix.clear();
    delay(50);
    YUEDU();
  }
  else if (buttonPushCounter % 5 == 2) {
    TEXT();
  }
  else if (buttonPushCounter % 5 == 3) {
    CLOCK();
  }
  else if (buttonPushCounter % 5 == 4) {
    FastLED.setBrightness(0);
    matrix.setBrightness(0);
    matrix.show();
    int buttonPushCounter = 0;
    digitalWrite(DATA_PIN, LOW);
  }
}
void YUEDU() {
  int i;
  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
    FastLED.show();
  }
}
void RBOW() {
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("output = ");
  Serial.println(outputValue);
  FastLED.setBrightness(outputValue);
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
  }
  EVERY_N_SECONDS( 10 ) {
    nextPattern();
  }
}
void TEXT() {
  matrix.setBrightness(outputValue);
  matrix.fillScreen(0);
  matrix.setCursor(x, 5);
  matrix.print(F("NICE TO MEET YOU"));
  if (--x < -50) {
    x = matrix.width();
    if (++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(150);
}
void CLOCK() {
  matrix.setBrightness(outputValue);
  matrix.fillScreen(0);
  matrix.setCursor(x, 5);
  matrix.print(F("17:18"));
  if (--x < -20) {
    x = matrix.width();
    if (++pass >= 3) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(150);
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow()
{
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
