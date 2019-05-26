#include <FastLED.h>
#define NUM_LEDS 80 //number of leds
#define DATA_PIN 14//data pin
#define BRIGHTNESS  255
CRGB leds[NUM_LEDS];
const int  buttonPin = 7;
int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;
String p;

void setup() {
  delay(2000);
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      buttonPushCounter++;
    } else {
      ;
    }
    delay(50);
  }
  lastButtonState = buttonState;
  if (buttonPushCounter  == 0) {
    shangsuo();
    if (Serial.available() > 0 )
    {
      p = Serial.readStringUntil('\n'); //serial - int  【0-79】
      Serial.println("p=" + p);
      if ( p == "1") {
        lightning(CRGB::White, 10, 5, 70);// V= color,simultaneous,cycle,speed
      } else if (p == "2") {
        lightning(CRGB::White, 20, 5, 65);
      } else if (p == "3") {
        lightning(CRGB::White, 30, 4, 60);
      } else if (p == "4") {
        lightning(CRGB::White, 30, 4, 60);
      } else if (p == "5") {
        lightning(CRGB::White, 30, 4, 60); 
      } else if (p == "6") {
        lightning(CRGB::White, 30, 3, 60); 
      } else if (p == "7") {
        lightning(CRGB::White, 30, 3, 60); 
      } else if (p == "8") {
        lightning(CRGB::White, 30, 2, 60); 
      } else if (p == "9") {
        lightning(CRGB::White, 30, 2, 60); 
      } else {
        bianli_2() ;
      }
      Serial.println("finished");
    }
  } else if (buttonPushCounter  == 1) {
    bianli();
    Ssuo_1();
    for (int a = 0; a < 3; a++) {
      lightning(CRGB::White, 10, 50, 100);
    }
    for (int b = 0; b < 3; b++) {
      lightning(CRGB::White, 20, 30, 50);// V= color,simultaneous,cycle,speed
    }
    for (int c = 0; c < 3; c++) {
      lightning(CRGB::White, 30, 20, 25);// V= color,simultaneous,cycle,speed
    }
    Serial.print("number of button pushes:  ");
    Serial.println(buttonPushCounter);
  } else  {
    buttonPushCounter  = 0;
    Serial.print("number of button pushes:  ");
    Serial.println(buttonPushCounter);
  }
}

void Ssuo_1() {
  for (int a = 0; a < 10; a++) {
    int i = random(NUM_LEDS);
    int j = random(NUM_LEDS);
    leds[i] = CRGB::White;
    leds[j] = CRGB::White;
    FastLED.show();
    FastLED.delay(500);
    FastLED.clear();
    FastLED.show();
    FastLED.delay(500);
  }
}
void bianli() {
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    leds[whiteLed] = CRGB::White;
    FastLED.show();
    delay(50);
    leds[whiteLed] = CRGB::Black;
  }
}
void shangsuo() {
  int i = random(NUM_LEDS);
  int j = random(NUM_LEDS);
  leds[i] = CRGB::White;
  leds[j] = CRGB::White;
  FastLED.show();
  FastLED.delay(500);
  FastLED.clear();
}
void bianli_2() {
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    leds[whiteLed] = CRGB::White;
    FastLED.show();
    delay(2);
    leds[whiteLed] = CRGB::Black;
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
