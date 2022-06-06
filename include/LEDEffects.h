#ifndef LED_EFFECTS_H
#define LED_EFFECTS_H
#include "FastLED.h"
#ifndef NUM_LEDS
#define NUM_LEDS 57
#define CHIPSET WS2812B
#define COLOR_ORDER GRB
#define DATA_PIN 27
#define VOLTS 5
#define MAX_MA 500
#define BRIGHTNESS 255
#endif
CRGB leds[NUM_LEDS];

static void initFastLED(){
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}
static int mixColors(CRGB::HTMLColorCode color1, CRGB::HTMLColorCode color2, const uint rand)
{
  int color;
  if (rand)
  {
    color = color1 - color2;
  }
  else
  {
    color = color1 + color2;
  }
  if (color == 0)
  {
    return CRGB::Gold;
  }
  return color;
}
static void straightLineMaxbrightness(CRGB::HTMLColorCode color, const size_t del = 30000)
{
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
        leds[i].setColorCode(color).maximizeBrightness();
        FastLED.show();
    }
    vTaskDelay(del / portTICK_PERIOD_MS);
}
static void doRandom(const size_t rounds, const size_t del)
{
  for (size_t i = 0; i < rounds; i++)
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
                    const int pulse = 6, uint forwardDir = 1, const uint swapDir = 0,
                    const uint8_t fade = 90)
{
  for (size_t i = 0; i < rounds; i++)
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
          leds[j - pulse].fadeLightBy(fade);
        }
        FastLED.show();
        vTaskDelay(del / portTICK_PERIOD_MS);
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
          leds[j + pulse].fadeLightBy(fade);
        }
        FastLED.show();
        vTaskDelay(del / portTICK_PERIOD_MS);
      }
    }
    forwardDir = swapDir ? !forwardDir : forwardDir;
  }
}
static void split(const size_t rounds, const size_t del, CRGB::HTMLColorCode color,
                  const int pulse = 10, const int pulseOffset = 1, const uint8_t fade = 90) // pulseOffset -> always the same atm
{
  for (size_t i = 0; i < rounds; i++)
  {
    const int splitP = NUM_LEDS / 2;
    for (int j = 0; j < (splitP + 1 + pulse); j++)
    {
      const int backwardOffset = splitP - j;
      const int forwardOffset = splitP + j;
      if (backwardOffset >= 0)
      {
        leds[backwardOffset].setColorCode(color);
      }
      if (forwardOffset < NUM_LEDS)
      {
        leds[forwardOffset].setColorCode(color);
      }
      if (j > pulseOffset)
      {
        leds[backwardOffset + pulse].fadeLightBy(fade);
        leds[forwardOffset - pulse].fadeLightBy(fade);
      }
      FastLED.show();
      vTaskDelay(del / portTICK_PERIOD_MS);
    }
  }
}
static void splash(const size_t rounds, const size_t del, CRGB::HTMLColorCode color1,
                   CRGB::HTMLColorCode color2, const uint rand, const uint spread = 10, const uint mix = 1,
                   const uint multiplier = 2)
{
  for (size_t i = 0; i < rounds; i++)
  {
    uint splitP = NUM_LEDS / 2;
    for (int j = 0; j < splitP + 1; j++)
    {
      leds[j].setColorCode(color1);
      leds[NUM_LEDS - j].setColorCode(color2);
      FastLED.show();
      vTaskDelay(del / portTICK_PERIOD_MS);
    }
    vTaskDelay(del / portTICK_PERIOD_MS);

    for (int i = 0; i < spread + 1; i++)
    {
      if (mix)
      {
        leds[splitP - i].setColorCode(mixColors(color1, color2, rand));
        leds[splitP + i].setColorCode(mixColors(color1, color2, rand));
        FastLED.show();
        vTaskDelay(del * multiplier / portTICK_PERIOD_MS);
      }
    }
    CRGB::HTMLColorCode temp;
    temp = color1;
    color1 = color2;
    color2 = temp;
  }
}
static CRGB::HTMLColorCode chooseColor(uint8_t cRand)
{
  switch (cRand & 31u)
  {
  case 1:
    return CRGB::Fuchsia;
  case 2:
    return CRGB::Red;
  case 3:
    return CRGB::Blue;
  case 4:
    return CRGB::Green;
  case 5:
    return CRGB::Yellow;
  case 6:
    return CRGB::DarkViolet;
  case 7:
    return CRGB::DarkGoldenrod;
  case 8:
    return CRGB::DeepPink;
  case 9:
    return CRGB::Amethyst;
  case 10:
    return CRGB::Lavender;
  case 11:
    return CRGB::DarkRed;
  case 12:
    return CRGB::Chocolate;
  case 13:
    return CRGB::Purple;
  case 14:
    return CRGB::Crimson;
  case 15:
    return CRGB::DarkOrange;
  case 16:
    return CRGB::FireBrick;
  case 17:
    return CRGB::DarkOrange;
  case 18:
    return CRGB::SpringGreen;
  case 19:
    return CRGB::DarkBlue;
  case 20:
    return CRGB::Ivory;
  case 21:
    return CRGB::DarkSeaGreen;
  case 22:
    return CRGB::RoyalBlue;
  case 23:
    return CRGB::Khaki;
  case 24:
    return CRGB::AliceBlue;
  case 25:
    return CRGB::BlueViolet;
  case 26:
    return CRGB::DarkGray;
  case 27:
    return CRGB::Azure;
  case 28:
    return CRGB::DarkOliveGreen;
  case 29:
    return CRGB::DarkCyan;
  case 30:
    return CRGB::DarkMagenta;
  case 31:
    return CRGB::CornflowerBlue;
  default:
    return CRGB::Gold; // case 0 whoops
  }
}
static CRGB::HTMLColorCode chooseFromFavColor(uint8_t cRand)
{
  switch (cRand & 0b111) // 2^3 = 8
  {
  case 1:
    return CRGB::Fuchsia;
  case 2:
    return CRGB::DarkBlue;
  case 3:
    return CRGB::Purple;
  case 4:
    return CRGB::Green;
  case 5:
    return CRGB::Yellow;
  case 6:
    return CRGB::DarkGoldenrod;
  case 7:
    return CRGB::Gold;
  default:
    return CRGB::Blue; // case 0 whoops
  }
}
#endif