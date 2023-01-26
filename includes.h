// Import required libraries
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
//SPIFFS LIBRARY ISSUE FOR NEWER MAC OS RESOLVED WITH:
//https://github.com/me-no-dev/arduino-esp32fs-plugin/issues/15#issuecomment-752965167
#include <FastLED.h>
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
#define DATA_PIN    0
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define NUM_LEDS    20
#define BRIGHTNESS  255
