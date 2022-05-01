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
                    const int pulse = 6, uint forwardDir = 1, const uint swapDir = 0)
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
    forwardDir = swapDir ? !forwardDir : forwardDir;
  }
}
static void split(const size_t rounds, const size_t del, CRGB::HTMLColorCode color,
                  const int pulse = 10, const int pulseOffset = 1) // pulseOffset -> always the same atm
{
  for (size_t i = 0; i < rounds - 1; i++)
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
        leds[backwardOffset + pulse].fadeLightBy(150);
        leds[forwardOffset - pulse].fadeLightBy(150);
      }
      FastLED.show();
      delay(del);
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
  long rand = random(LONG_MAX);
  CRGB::HTMLColorCode colorCode;
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
  // Serial.println((rand & 0b1));
  // Serial.print("bitRands ");
  // Serial.print(!!(rand & 0xF0));
  // Serial.println(!!(rand & 0xF00));
  // Serial.print("clamped ");
  // Serial.println(min(max((rand & 0b1111), (long)6), (long)11));
  // Serial.println("End");
  // delay(1000);
  switch (cRand)
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
  case 12:
    colorCode = CRGB::Chocolate;
    break;
  case 13:
    colorCode = CRGB::Purple;
    break;
  case 14:
    colorCode = CRGB::Crimson;
    break;
  case 15:
    colorCode = CRGB::DarkOrange;
    break;
  case 16:
    colorCode = CRGB::FireBrick;
    break;
  case 17:
    colorCode = CRGB::Silver;
    break;
  case 18:
    colorCode = CRGB::SpringGreen;
    break;
  case 19:
    colorCode = CRGB::BlanchedAlmond;
    break;
  case 20:
    colorCode = CRGB::Ivory;
    break;
  case 21:
    colorCode = CRGB::DarkSeaGreen;
    break;
  case 22:
    colorCode = CRGB::Wheat;
    break;
  default:
    return;
  }
  rand = rand >> 8;
  uint8_t i = random8(1, 3);
  if (rand & 0b1)
  {

    doFlame(10 * i, 20 / i, colorCode, min(max((rand & 0b1111), (long)6), (long)11), !!(rand & 0xF0), !!(rand & 0xF00));
  }
  else
  {
    split(10 * i, 20 / i, colorCode, min(max((rand & 0b1111), (long)6), (long)11));
  }
}
