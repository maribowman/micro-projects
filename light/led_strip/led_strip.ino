#include <FastLED.h>

#define LED_PIN D4
#define NUM_LEDS 50
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define BRIGHTNESS 128

#define TEMPERATURE_1 Tungsten40W
#define TEMPERATURE_2 ClearBlueSky
#define DISPLAYTIME 5
#define BLACKTIME 1

struct CRGB leds [NUM_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  for(int bright = 10; bright <= 255; bright+=5) {
    FastLED.setBrightness(bright);
    for(int index = 0; index < NUM_LEDS; index++) {
      leds[index] = CHSV(HUE_YELLOW, sat8, bright);;
      FastLED.show();
    }
    delay(100);
  }

  //CHSV colors (hue, sat, bri) 
  
  for(int bright = 255; bright >= 0; bright-=5) {
    FastLED.setBrightness(bright);
    for(int index = 0; index < NUM_LEDS; index++) {
      leds[index] = CRGB();
      FastLED.show();
    }
    delay(100);
  }
}

//void loop()
//{
//  // draw a generic, no-name rainbow
//  static uint8_t starthue = 0;
//  fill_rainbow( leds + 5, NUM_LEDS - 5, --starthue, 20);
//
//  // Choose which 'color temperature' profile to enable.
//  uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);
//  if( secs < DISPLAYTIME) {
//    FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
//    leds[0] = TEMPERATURE_1; // show indicator pixel
//  } else {
//    FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
//    leds[0] = TEMPERATURE_2; // show indicator pixel
//  }
//
//  // Black out the LEDs for a few secnds between color changes
//  // to let the eyes and brains adjust
//  if( (secs % DISPLAYTIME) < BLACKTIME) {
//    memset8( leds, 0, NUM_LEDS * sizeof(CRGB));
//  }
//  
//  FastLED.show();
//  FastLED.delay(8);
//}
