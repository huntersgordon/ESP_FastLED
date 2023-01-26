// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
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

//GPS

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin,TXPin);

CRGB leds[NUM_LEDS];

// Replace with your network credentials
bool rainbow = false;
bool fade = false;
bool strobe = false;
bool blinky = true;
int currFade = 255;
int currSpeed = 0;
bool down = true;
uint16_t fadeMs = millis();
uint16_t strobeMs = millis();
const char* ssid = "RUSTPUTIA";
const char* password = "pass";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String processor(const String& var) {
  //Serial.println(var);
  if (var == "BUTTONPLACEHOLDER") {
    String buttons = "";
    buttons += "<h4>Output - GPIO 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\" " + outputState(5) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String outputState(int output) {
  if (digitalRead(output)) {
    return "checked";
  }
  else {
    return "";
  }
}


void setSolid(int R, int G, int B, bool front) {
  if (!front) {
    for (int i = (NUM_LEDS) / 2; i < NUM_LEDS; i++) {
      leds[i].setRGB( R, G, B);
    }
  }
  else {
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      leds[i].setRGB( R, G, B);
    }
  }
  FastLED.show();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);
  Serial.println("AsdfsdfIFFS");
  ss.begin(GPSBaud);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  setSolid( 255,  255, 255, true);
  setSolid( 255,  255, 255, false);

  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // For testing, set ssid and pass to your home wifi's info
  //  Wifi.begin(ssid,password);
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(1000);
  //    Serial.println("Connecting to WiFi..");
  //  }
  // Print ESP Local IP Address
  //Serial.println(WiFi.localIP());

  IPAddress local_IP(192, 168, 1, 1);
  IPAddress gateway(192, 168, 4, 9);
  IPAddress subnet(255, 255, 255, 0);
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  WiFi.softAP(ssid, NULL);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/rainbow", HTTP_GET, [](AsyncWebServerRequest * request) {
    strobe = false;
    rainbow = true;
    fade = false;
  });

  server.on("/fade", HTTP_GET, [](AsyncWebServerRequest * request) {
    strobe = false;
    rainbow = false;
    fade = true;
    fadeMs = millis();
  });


  server.on("/strobe", HTTP_GET, [](AsyncWebServerRequest * request) {
    strobe = true;
    rainbow = false;
    fade = false;
    strobeMs = millis();
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/iro.min.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/iro.min.js", "text/javascript");
  });

  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send(200, "text/plain", String(currSpeed));
  });

  server.on("/front", HTTP_GET, [] (AsyncWebServerRequest * request) {
    rainbow = false;
    strobe = false;
    fade = false;
    String R;
    String G;
    String B;
    int paramsNr = 3;
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter* p = request->getParam(i);
      Serial.print("Param name: ");
      Serial.println(p->name());
      if (p->name() == "R") {
        R = p->value();
      }
      if (p->name() == "G") {
        G = p->value();
      }
      if (p->name() == "B") {
        B = p->value();
      }
      Serial.print("Param value: ");
      Serial.println(p->value());
      Serial.println("------");
    }
    setSolid(R.toInt(), G.toInt(), B.toInt(), true);
    request->send(200, "text/plain", "OK");
  });

  server.on("/back", HTTP_GET, [] (AsyncWebServerRequest * request) {
    rainbow = false;
    strobe = false;
    fade = false;
    String R;
    String G;
    String B;
    int paramsNr = 3;
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter* p = request->getParam(i);
      Serial.print("Param name: ");
      Serial.println(p->name());
      if (p->name() == "R") {
        R = p->value();
      }
      if (p->name() == "G") {
        G = p->value();
      }
      if (p->name() == "B") {
        B = p->value();
      }
      Serial.print("Param value: ");
      Serial.println(p->value());
      Serial.println("------");
    }
    setSolid(R.toInt(), G.toInt(), B.toInt(), false);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop()
{

  while(ss.available() > 0) {
    if (gps.encode(ss.read())){
      if (gps.speed.isValid()){
        currSpeed = floor(gps.speed.mph());
      }
    }
  }

  
  if (strobe) {
    rainbow = false;
    fade = false;
    if (millis() - strobeMs > 50) {
      strobeMs = millis();
      blinky = !blinky;
    }
    party();
    FastLED.show();
  }
  if (rainbow) {
    strobe = false;
    fade = false;
    pride();
    FastLED.show();
  }
  if (fade) {
    strobe = false;
    rainbow = false;
    if (millis() - fadeMs > 2) {
      fadeMs = millis();
      faded();
    }
    FastLED.show();
  }
}

void faded() {
  if (currFade == 0) {
    down = false;
  }
  if (currFade > 254) {
    down = true;
  }
  if (down) {
    currFade -= 1;
  } else {
    currFade += 1;
  }

  FastLED.setBrightness(currFade);


}

void party() {
  if (blinky) {
    for (int i = 0; i < NUM_LEDS; i++) {
      FastLED.setBrightness(255);
    }
  } else {
    for (int i = 0; i < NUM_LEDS; i++) {
      FastLED.setBrightness(0);
    }
  }
}
void pride()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend( leds[pixelnumber], newcolor, 64);
  }
}
