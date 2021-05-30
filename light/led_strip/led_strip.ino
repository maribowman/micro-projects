#include <FastLED.h>

#define NUM_LEDS 5
#define LED_PIN D4

CRGB leds [NUM_LEDS];

void setup() {
//  Serial.begin(115200);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  for(int i = 0; i < 5; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::White;
  }
//  
//  leds[0] = CRGB::Red;
//  FastLED.show();
//  delay(500);  
//  leds[1] = CRGB::Blue;
//  FastLED.show();
//  delay(500);
//  leds[2] = CRGB::Green;
//  FastLED.show();
//  delay(500);
}
