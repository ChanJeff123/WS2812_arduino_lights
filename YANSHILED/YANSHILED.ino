#include "FastLED.h"
FASTLED_USING_NAMESPACE

#define DATA_PIN    7
const int buttonPin = 2;
int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    90
String p;

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND  120
CRGB leds[NUM_LEDS];

uint16_t ledsData[NUM_LEDS][4];
uint16_t pick;

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  delay(2000);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void(* resetFunc) (void) = 0;

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { christmasSparkles,  christmasSparklesRG, christmasSparklesBP , confetti};
uint8_t gCurrentPatternNumber = 0;
uint8_t gHue = 0;

void loop() {
  gPatterns[gCurrentPatternNumber]();
  FastLED.delay(120 / FRAMES_PER_SECOND);
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
  }
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      buttonPushCounter++;
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
      leds[0] = CHSV(0, 0, 255);
      FastLED.show();
      delay(100);
      leds[0] = CRGB::Black;
      FastLED.show();
      delay(100);
    } else {
      ;
    }
    delay(50);
  }
  lastButtonState = buttonState;
  if (buttonPushCounter   == 0) {
    if (Serial.available() > 0 )
    {
      p = Serial.readStringUntil('\n');
      Serial.println("p=" + p);
      if ( p == "1") {
        gCurrentPatternNumber = 0;
      } else if (p == "2") {
        gCurrentPatternNumber = 1;
      } else if (p == "3") {
        gCurrentPatternNumber = 2;
      } else if (p == "4") {
        lightning(CRGB::White, 50, 20, 100);// V= color,simultaneous,cycle,speed;
      }  else {
        gCurrentPatternNumber = 3;
      }
      Serial.print(gCurrentPatternNumber);
      Serial.println("finished");
      FastLED.show();
    }
  } else if (buttonPushCounter == 1) {
    routine();
  }
  else if (buttonPushCounter == 2) {
    FastLED.clear();
    resetFunc();
  }
}

void routine() {
  EVERY_N_SECONDS( 10 ) {
    nextPattern();
  }
}
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
void nextPattern()
{
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void confetti()
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(255), random8(0, 1), random8(48, 255));
}

void christmasSparkles() {
  CRGB sparkleBgColor = CHSV(0, 0, 0);
  EVERY_N_MILLISECONDS(40) {
    if ( random8() < 20 ) {
      pick = random16(NUM_LEDS);
      if (ledsData[pick][3] == 0 ) {
        ledsData[pick][3] = 35;
        uint8_t randomPick = random8(5);
        if (randomPick == 0) {
          ledsData[pick][0] = 178;
          ledsData[pick][1] = 0;
          ledsData[pick][2] = 210;
        }
        if (randomPick == 1) {
          ledsData[pick][0] = 10;
          ledsData[pick][1] = 0;
          ledsData[pick][2] = 240;
        }
        if (randomPick == 2) {
          ledsData[pick][0] = 0;
          ledsData[pick][1] = 0;
          ledsData[pick][2] = 255;
        }
        if (randomPick == 3) {
          ledsData[pick][0] = 35;
          ledsData[pick][1] = 0;
          ledsData[pick][2] = 245;
        }
        if (randomPick == 4) {
          ledsData[pick][0] = 190;
          ledsData[pick][1] = 0;
          ledsData[pick][2] = 238;
        }
        leds[pick] = CHSV(ledsData[pick][0], ledsData[pick][1], ledsData[pick][2]);
      }
    }
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
      if (ledsData[i][3] == 0) {
        leds[i] = sparkleBgColor;
      } else {
        CHSV hsv = rgb2hsv_approximate(leds[i]);
        EVERY_N_MILLISECONDS(38) {
          ledsData[i][0] = hsv.hue - 1;
        }
        ledsData[i][2] = scale8(ledsData[i][2], 245);
        leds[i] = CHSV(ledsData[i][0], ledsData[i][1], ledsData[i][2]);
        ledsData[i][3] = ledsData[i][3] - 1;
      }
    }
  }
}

void christmasSparklesRG() {
  CRGB sparkleBgColor = CHSV(0, 0, 0);
  EVERY_N_MILLISECONDS(40) {
    if ( random8() < 90 ) {
      pick = random16(NUM_LEDS);
      if (ledsData[pick][3] == 0 ) {
        ledsData[pick][3] = 65;
        uint8_t randomPick = random8(2);
        if (randomPick == 0) {
          ledsData[pick][0] = 16;
          ledsData[pick][1] = 0;
          ledsData[pick][2] = 242;
        }
        if (randomPick == 1) {
          ledsData[pick][0] = 96;
          ledsData[pick][1] = 0;
          ledsData[pick][2] = 255;
        }
        leds[pick] = CHSV(ledsData[pick][0], ledsData[pick][1], ledsData[pick][2]);
      }
    }
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
      if (ledsData[i][3] == 0) {
        leds[i] = sparkleBgColor;
      } else {
        CHSV hsv = rgb2hsv_approximate(leds[i]);
        EVERY_N_MILLISECONDS(50) {
          ledsData[i][0] = hsv.hue - 1;
        }
        ledsData[i][2] = scale8(ledsData[i][2], 253);
        leds[i] = CHSV(ledsData[i][0], ledsData[i][1], ledsData[i][2]);
        ledsData[i][3] = ledsData[i][3] - 1;
      }
    }
  }
}

void christmasSparklesBP() {
  CRGB sparkleBgColor = CHSV(0, 0, 0);
  EVERY_N_MILLISECONDS(40) {
    if ( random8() < 180 ) {
      pick = random16(NUM_LEDS);
      if (ledsData[pick][3] == 0 ) {
        ledsData[pick][3] = 20;
        uint8_t randomPick = random8(3);
        if (randomPick == 0) {
          ledsData[pick][0] = 165;
          ledsData[pick][1] = 0;
          ledsData[pick][2] = 230;
        }
        if (randomPick == 1) {
          ledsData[pick][0] = 200;
          ledsData[pick][1] = 0;
          ledsData[pick][2] = 240;
        }
        if (randomPick == 2) {
          ledsData[pick][0] = 130;
          ledsData[pick][1] = 0;
          ledsData[pick][2] = 255;
        }
        leds[pick] = CHSV(ledsData[pick][0], ledsData[pick][1], ledsData[pick][2]);
      }
    }
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
      if (ledsData[i][3] == 0) {
        leds[i] = sparkleBgColor;
      } else {
        CHSV hsv = rgb2hsv_approximate(leds[i]);
        EVERY_N_MILLISECONDS(20) {
          ledsData[i][0] = hsv.hue - 1;
        }
        ledsData[i][2] = scale8(ledsData[i][2], 242);
        leds[i] = CHSV(ledsData[i][0], ledsData[i][1], ledsData[i][2]);
        ledsData[i][3] = ledsData[i][3] - 1;
      }
    }
  }
}
void lightning(CRGB c, int simultaneous, int cycles, int speed) {
  int flashes[simultaneous];
  for (int i = 0; i < cycles; i++) {
    for (int j = 0; j < simultaneous; j++) {
      int idx = random(NUM_LEDS);
      flashes[j] = idx;
      leds[idx] = c ? c : randomColor();
    }
    FastLED.show();
    delay(speed);
    for (int s = 0; s < simultaneous; s++) {
      leds[flashes[s]] = CRGB::Black;
    }
    delay(speed);
  }
}
CRGB Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
CRGB randomColor() {
  return Wheel(random(256));
}
