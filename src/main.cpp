#include <Arduino.h>
#include "FastLED.h"
#include "WiFi.h"
#include "WiFiServer.h"
#include "LEDEffects.h"
#include "esp_system.h"
#include "wifi_secrets.h"


IPAddress local_IP(192, 168, 8, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(1, 1, 1, 1);
IPAddress secondaryDNS(1, 0, 0, 1);
WiFiServer server(24);
TaskHandle_t LEDTask = NULL;
TaskHandle_t WiFiTask = NULL;

// String getMacAddress() {
// 	uint8_t baseMac[6];
// 	// Get MAC address for WiFi station
// 	esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
// 	char baseMacChr[18] = {0};
// 	sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
// 	return String(baseMacChr);
// }
static void callStraightLine(void *parameters)
{
  straightLineMaxbrightness(*((CRGB::HTMLColorCode *)parameters));
  vTaskResume(LEDTask);
  vTaskDelete(NULL);
}
static void runWiFiServer(void *paramaters)
{
  for (;;)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("WiFi Connected, Server waiting for client request");
      WiFiClient client = server.available();
      if (client)
      {
        uint8_t data[30];
        Serial.println("New client connected");
        /* check client is connected */
        while (client.connected())
        {
          TaskHandle_t clientTask = NULL;
          if (client.available())
          {
            int len = client.read(data, 30);
            if (len < 30)
            {
              data[len] = '\0';
            }
            else
            {
              data[29] = '\0';
            }
            Serial.print("Client sent: ");
            Serial.println((char *)data);
            String *str = new String((char *)data);
            CRGB::HTMLColorCode color = CRGB::Brown;
            if (str->compareTo("ping;") == 0)
            {
              Serial.println("pong");
            }
            else
            {
              if (str->compareTo("blue;") == 0)
              {
                color = CRGB::Blue;
              }
              else if (str->compareTo("green;") == 0)
              {
                color = CRGB::Green;
              }
              else if (str->compareTo("red;") == 0)
              {
                color = CRGB::Red;
              }
              else if (str->compareTo("gold;") == 0)
              {
                color = CRGB::Gold;
              }
              if (clientTask != NULL)
              {
                vTaskDelete(clientTask);
              }
              vTaskSuspend(LEDTask);
              xTaskCreate(callStraightLine, "clientTask", 5000, &color, 2, &clientTask);
            }
            client.flush();
          }
        }
        Serial.println("Client disconnected");
      }
      vTaskDelay(WIFI_TIMEOUT_MS / portTICK_PERIOD_MS);
      continue;
    }
    Serial.println("Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
    ulong startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS)
    {
    }
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Failed to connect to WiFi");
      vTaskDelay(WIFI_TIMEOUT_MS / portTICK_PERIOD_MS);
    }

    Serial.print("WiFi Connected: ");
    Serial.println(WiFi.localIP());
    server.begin();
  }
}

static void runLeds(void *parameters)
{
  for (;;)
  {
    int rand = random(LONG_MAX);
    // Serial.println("Start");
    uint8_t cRand = rand & 0xFF; // first 8 bits reserved for color random
    // Serial.print("cRand ");
    // Serial.println(cRand);
    // Serial.print("cRand color");
    // Serial.println(cRand & 32);
    // Serial.print("Rand ");
    // Serial.println(rand);
    // Serial.print("funcRand ");
    // rand >>= 8;
    // Serial.println(rand & 0b11);
    // rand >>= 2;
    // Serial.print("clamped ");
    // Serial.println(min(max((rand & 0b1111), 6), 11));
    // Serial.print("bitRands ");
    // Serial.print((rand >>= 4) & 0b1);
    // Serial.println(rand & 0b10);
    // Serial.println("End");
    // delay(1000);
    CRGB::HTMLColorCode colorCode = chooseColor(cRand);
    rand >>= 8;
    uint8_t i = random8(1, 3);
    uint8_t fRand = rand & 0b11; // 1st 2 bits
    rand >>= 2;
    if (fRand == 0)
    {

      doFlame(10 * i, 20 / i, colorCode, min(max((rand & 0b1111), 6), 11),
              (rand >> 4) & 0b1, (rand >> 5) & 0b1, min(max((((rand >> 6) & 0xF) * 15), 90), 200));
    }
    else if (fRand == 1)
    {
      split(10 * i, 20 / i, colorCode, min(max((rand & 0b1111), 6), 11),
            1, min(max((((rand >> 4) & 0xF) * 15), 90), 200));
    }
    else if (fRand == 2)
    {
      splash(10 * i, 20 / i, colorCode, chooseColor((rand >> 8) & 0xFF), rand & 0b1, 15);
    }
    else if (fRand == 3)
    {
      if (i == 1)
      {
        doRandom(30, 100);
      }
      else
      {
        straightLineMaxbrightness(chooseFromFavColor(cRand), 30000);
      }
    }
  }
}
void setup()
{
  initFastLED();
  Serial.begin(115200);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("Failed to configure");
  }
  xTaskCreate(runLeds, "RunLeds", 5000, NULL, 2, &LEDTask);
  xTaskCreatePinnedToCore(runWiFiServer, "WiFi", 5000, NULL, 1, &WiFiTask, CONFIG_ARDUINO_RUNNING_CORE);
}

void loop()
{
  // uint8_t mac;
  // WiFi.macAddress(&mac);
  // Serial.println(mac);
  // delay(1000);
}
