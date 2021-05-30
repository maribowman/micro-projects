#include <FastLED.h>


#define NUM_LEDS 5
#define LED_PIN D4

CRGB leds [NUM_LEDS]

void setup() {
//  Serial.begin(115200);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

//void loop() {
//  leds[0] = CRGB::Red;
//  FastLED.show();
//  delay(500);  
//  leds[1] = CRGB::Blue;
//  FastLED.show();
//  delay(500);
//  leds[2] = CRGB::Green;
//  FastLED.show();
//  delay(500);
//}

void loop() { 
  // Turn the LED on, then pause
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
}
