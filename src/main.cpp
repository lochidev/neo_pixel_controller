#include <Arduino.h>
#include "FastLED.h"

#define NUM_LEDS 55
#define CHIPSET WS2812B
#define COLOR_ORDER GRB
#define DATA_PIN 27
#define VOLTS 5
#define MAX_MA 500
#define BRIGHTNESS 200

CRGB leds[NUM_LEDS];

static void doRandom(const size_t rounds, const size_t del)
{
  for (size_t i = 0; i < rounds - 1; i++)
  {
    for (size_t j = 0; j < NUM_LEDS; j++)
    {
      leds[j].setRGB(random8(), random8(), random8());
    }
    FastLED.show();
    delay(del);
  }
}
static void doFlame(const size_t rounds, const size_t del, CRGB::HTMLColorCode color,
                    const int pulse, const uint forwardDir = 1)
{
  for (size_t i = 0; i < rounds - 1; i++)
  {
    if (forwardDir)
    {
      for (size_t j = 0; j < NUM_LEDS + pulse; j++)
      {
        if (j < NUM_LEDS)
        {
          leds[j].setColorCode(color);
        }
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
      for (int j = NUM_LEDS; j > -(pulse + 1); j--)
      {
        if (j >= 0)
        {
          leds[j].setColorCode(color);
        }
        if (j < (NUM_LEDS - (pulse - 1)))
        {
          leds[j + pulse].fadeLightBy(150);
        }
        FastLED.show();
        delay(del);
      }
    }
  }
}
void setup()
{
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  pinMode(DATA_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  uint8_t rand = random8(1, 12);
  CRGB::HTMLColorCode colorCode;
  switch (rand)
  {
  case 1:
    doRandom(30, 100);
    return;
  case 2:
    colorCode = CRGB::Red;
    break;
  case 3:
    colorCode = CRGB::Blue;
    break;
  case 4:
    colorCode = CRGB::Green;
    break;
  case 5:
    colorCode = CRGB::Yellow;
    break;
  case 6:
    colorCode = CRGB::HotPink;
    break;
  case 7:
    colorCode = CRGB::LightGoldenrodYellow;
    break;
  case 8:
    colorCode = CRGB::DeepPink;
    break;
  case 9:
    colorCode = CRGB::Amethyst;
    break;
  case 10:
    colorCode = CRGB::Gold;
    break;
  case 11:
    colorCode = CRGB::DarkRed;
    break;
  }
  doFlame(10, 20, colorCode, random8(6, 11), !!random8(0, 2));
}
