#include <Arduino.h>
#include "FastLED.h"
#include <random>

#define NUM_LEDS 60
#define CHIPSET WS2812B
#define COLOR_ORDER GRB
#define DATA_PIN 2
#define VOLTS 5
#define MAX_MA 500
#define BRIGHTNESS 100

CRGB leds[NUM_LEDS];

void setup()
{
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  pinMode(DATA_PIN, OUTPUT);
}
static inline void do_random(const size_t rounds, const size_t del)
{
  for (size_t i = 0; i < rounds - 1; i++)
  {
    for (size_t j = 0; j < 59; j++)
    {
      leds[j].setRGB(random8(), random8(), random8());
    }
    FastLED.show();
    delay(del);
  }
}
static inline void do_flame(const size_t rounds, const size_t del, CRGB::HTMLColorCode color,
                            const size_t pulse, const bool forwardDir = true)
{
  for (size_t i = 0; i < rounds - 1; i++)
  {
    if (forwardDir)
    {
      for (size_t j = 0; j < NUM_LEDS + pulse; j++)
      {
        leds[j].setColorCode(color);
        if (j > (pulse - 1))
        {
          leds[j - pulse].fadeLightBy(150);
        }
        FastLED.show();
        delay(del);
      }
    }
    else
    {
      for (size_t j = NUM_LEDS; j > -pulse; j--)
      {
        leds[j].setColorCode(color);
        if (j < (NUM_LEDS - (pulse - 1)))
        {
          leds[j + pulse].fadeLightBy(90);
        }
        FastLED.show();
        delay(del);
      }
    }
  }
}
void loop()
{
  uint8_t rand = random8(1, 4);
  switch (rand)
  {
  case 1:
    do_random(60, 100);
    break;
  case 2:
    do_flame(30, 20, CRGB::Red, 6, random8());
    break;
  case 3:
    do_flame(30, 20, CRGB::Blue, 6, random8());
    break;
  case 4:
    do_flame(30, 20, CRGB::Green, 6, random8());
    break;
  }
}