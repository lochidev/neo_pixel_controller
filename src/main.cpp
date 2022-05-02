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
          leds[j + pulse].fadeLightBy(fade);
        }
        FastLED.show();
        delay(del);
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
      delay(del);
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
      delay(del);
    }
    delay(del);
    for (int i = 0; i < spread + 1; i++)
    {
      if (mix)
      {
        leds[splitP - i].setColorCode(mixColors(color1, color2, rand));
        leds[splitP + i].setColorCode(mixColors(color1, color2, rand));
        FastLED.show();
        delay(del * multiplier);
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
  switch (cRand)
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
    return CRGB::HotPink;

  case 7:
    return CRGB::LightGoldenrodYellow;

  case 8:
    return CRGB::DeepPink;

  case 9:
    return CRGB::Amethyst;

  case 10:
    return CRGB::Gold;

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
    return CRGB::Silver;

  case 18:
    return CRGB::SpringGreen;

  case 19:
    return CRGB::BlanchedAlmond;

  case 20:
    return CRGB::Ivory;

  case 21:
    return CRGB::DarkSeaGreen;

  case 22:
    return CRGB::Wheat;

  default:
    return CRGB::Lavender;
  }
}
void setup()
{
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.begin(115200);
}

void loop()
{
  int rand = random(LONG_MAX);

  // Serial.println("Start");
  uint8_t cRand = rand & 0xFF;
  // Serial.print("cRand 1 ");
  // Serial.println(cRand);
  if (cRand > 22)
  {
    cRand &= 0x0F;
  }
  // Serial.print("cRand ");
  // Serial.println(cRand);
  // Serial.print("Rand ");
  // Serial.println(rand);
  // Serial.print("funcRand ");
  // rand = rand >> 8;
  // Serial.println(rand & 0b11);
  // Serial.print("bitRands ");
  // Serial.print(!!(rand & 0x20));
  // Serial.println(!!(rand & 0x10));
  // Serial.print("clamped ");
  // Serial.println(min(max((((rand & 0xF00) >> 8) * 15), 120), 200));
  // Serial.println("End");
  // delay(1000);
  CRGB::HTMLColorCode colorCode = chooseColor(cRand);
  rand = rand >> 8;
  uint8_t i = random8(1, 3);
  uint8_t fRand = rand & 0b11;
  if (fRand == 0)
  {

    doFlame(10 * i, 20 / i, colorCode, min(max((rand & 0b1111), 6), 11),
            !!(rand & 0x10), !!(rand & 0x20), min(max((((rand & 0xF00) >> 8) * 15), 90), 200));
  }
  else if (fRand == 1)
  {
    split(10 * i, 20 / i, colorCode, min(max((rand & 0b1111), 6), 11),
          1, min(max((((rand & 0xF00) >> 8) * 15), 90), 200));
  }
  else if (fRand == 2)
  {
    splash(10 * i, 20 / i, colorCode, chooseColor((rand >> 8) & 0xFF), !!(rand & 0x100), 15);
  }
  else if (fRand == 3)
  {
    doRandom(30, 100);
  }
}
