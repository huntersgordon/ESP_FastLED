void initiatePageRoutes() {

  // Route index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false);
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

}

//void notifyClients() {
// //WHERE ledState is the state that we want to inform the clients
//  ws.textAll(String(ledState));
//}

//SOCKET EVENT HANDLERS
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  Serial.println("Socket message received");
  Serial.println((char*)data);
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    char *ret = (char*)data;
    if (strcmp(ret, "strobe") == 0) {
      FastLED.setBrightness(255);
      strobeMs = millis();
      effect = "strobe";
    }
    else if (strcmp(ret, "fade") == 0) {
      FastLED.setBrightness(255);
      fadeMs = millis();
      effect = "fade";
    }
    else if (strcmp(ret, "rainbow") == 0) {
      FastLED.setBrightness(255);
      effect = "rainbow";
    }
    //Otherwise the WS might have JSON. We will check.
    else {
      effect = "none";
      const uint8_t size = JSON_OBJECT_SIZE(4);
      StaticJsonDocument<size> json;
      DeserializationError err = deserializeJson(json, data);
      if (err) {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.c_str());
        return;
      }
      else {
        const uint8_t R = json["R"];
        const uint8_t G = json["G"];
        const uint8_t B = json["B"];
        const bool front = json["front"];

        setSolid(R, G, B, front);
      }

    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
