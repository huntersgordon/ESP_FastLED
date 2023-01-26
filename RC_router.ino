#include "includes.h"

CRGB leds[NUM_LEDS];

char* effect = "none";
bool blinky = true;
int currFade = 255;
bool down = true;
uint16_t fadeMs = millis();
uint16_t strobeMs = millis();

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup() {
  // Serial port for debugging
//  Serial.begin(115200);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  //SET LEDS TO ALL SOLID COLORS ON STARTUP
  setSolid( 255,  255, 255, true);
  setSolid( 255,  255, 255, false);

  //AsyncSetup.ino: Setup SPIFFS & Access Point
  AsyncSetup();

  //Routes.ino: Handle requests to index.html, style.css, etc.
  initiatePageRoutes();

  //Handle requests from client
  initWebSocket();
  // Start server
  server.begin();
}

void loop()
{
  //cleanupClients() Handles if browsers do not
  //correctly close the WebSocket connection, even
  //When close() is called in Javascript.
  ws.cleanupClients();
  //IN LED_Functions, check if the global
  //Variables were changed
  updateLEDS();
}
