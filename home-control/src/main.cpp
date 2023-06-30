/*
  lib_deps:
	fastled/FastLED@^3.6.0
	adafruit/DHT sensor library@^1.4.4
	adafruit/Adafruit Unified Sensor@^1.1.9
	ottowinter/ESPAsyncWebServer-esphome@^3.0.0
*/

#include "headers/includes.h"

CRGB leds_room[NUM_ROOM];

const char* ssid = "cablelink_0823061";
const char* password = "kkYo57icS5(65ZYU";

//DHT Sensor
DHT dht(DHT_PORT, DHT_TYPE);
unsigned long previousMillis = 0;
const unsigned long interval = 5000;  // Interval between readings in milliseconds

//Set webserver to port 80
AsyncWebServer server(80);

String header;
String outputRoomState = "off";

//global vars
CRGB g_LEDColor = CRGB::BlueViolet;
int g_animationCode = 0;
CRGB g_colorLivingRoom;
CRGB g_colorKitchen;

int room = 0;
float t = 0.0;
float h = 0.0;

String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

//Prototypes
void turnRoomOn(int animation, int start, int end, CRGB color);
void turnRoomOff(int animation, int start, int end);
void updateLEDColors(String hexCode, int start, int end, CRGB* color);
void saveToEEPROM();
void loadFromEEPROM();

void setup() {
  //system_update_cpu_freq(SYS_CPU_160MHZ);
  EEPROM.begin(512);

  Serial.begin(9600);
  Serial.println();
  Serial.println();
  FastLED.addLeds<LED_TYPE, ROOM_PORT, COLOR_ORDER>(leds_room, NUM_ROOM).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  dht.begin();

  WiFi.begin(ssid, password);

  while(WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  server.on("/roomLivingOn", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("[LIGHTS] Turning ON living room");
    turnRoomOn(g_animationCode, 0, NUM_LIVING_ROOM, g_colorLivingRoom);
  });

  server.on("/roomLivingOff", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("[LIGHTS] Turning OFF living room");
    turnRoomOff(g_animationCode, 0, NUM_LIVING_ROOM);
  });

  server.on("/roomKitchenOn", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("[LIGHTS] Turning ON kitchen");
    turnRoomOn(g_animationCode, NUM_LIVING_ROOM, NUM_KITCHEN + NUM_LIVING_ROOM, g_colorKitchen);
  });

  server.on("/roomKitchenOff", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("[LIGHTS] Turning OFF kitchen");
    turnRoomOff(g_animationCode, NUM_LIVING_ROOM, NUM_KITCHEN + NUM_LIVING_ROOM);
  });

  server.on("/setAnimation", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("animation")) {
      String animation = request->getParam("animation")->value();
      Serial.println("[ANIMATION] Code: " + animation);
      g_animationCode = animation.toInt();
    }
  });

  server.on("/setColorLivingRoom", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("color")) {
      String hexCode = request->getParam("color")->value();
      Serial.println("[SET-COLOR-LIVING-ROOM] Color: " + hexCode);
      updateLEDColors(hexCode, 0, NUM_LIVING_ROOM, &g_colorLivingRoom);
    }
  });

  server.on("/setColorKitchen", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("color")) {
      String hexCode = request->getParam("color")->value();
      Serial.println("[SET-COLOR-KITCHEN] Color: " + hexCode);
      updateLEDColors(hexCode, NUM_LIVING_ROOM, NUM_KITCHEN + NUM_LIVING_ROOM, &g_colorKitchen);
    }
  });

  server.on("/saveEEPROM", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("[EEPROM] Saving values to EEPROM");
    saveToEEPROM();
  });

  server.on("/loadEEPROM", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("[EEPROM] Loading values from EEPROM");
    loadFromEEPROM();
  });

  server.begin();
}

void loop() {  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    float newT = dht.readTemperature();

    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      t = newT;
    }

    float newH = dht.readHumidity();

    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      h = newH;
    }
  }
}

void saveToEEPROM() {
  Serial.println("[EEPROM] Putting g_animationCode");
  EEPROM.put(EEPROM_ANIMATION_ADDRESS, g_animationCode);

  Serial.println("[EEPROM] Putting g_colorLivingRoom");
  EEPROM.put(EEPROM_COLOR_LIVING_ADDRESS, g_colorLivingRoom);

  Serial.println("[EEPROM] Putting g_colorKitchen");
  EEPROM.put(EEPROM_COLOR_KITCHEN_ADDRESS, g_colorKitchen);

  Serial.println("[EEPROM] Commiting data to EEPROM");
  EEPROM.commit();
}

void loadFromEEPROM() {

  EEPROM.get(EEPROM_ANIMATION_ADDRESS, g_animationCode);
  if(g_animationCode > -1) {
    Serial.println("[EEPROM] Loading animation from EEPROM");

    EEPROM.get(EEPROM_COLOR_LIVING_ADDRESS, g_colorLivingRoom);
    Serial.println("[EEPROM] Loading living room from EEPROM");
    turnRoomOn(g_animationCode, 0, NUM_LIVING_ROOM, g_colorLivingRoom);
    
    EEPROM.get(EEPROM_COLOR_KITCHEN_ADDRESS, g_colorKitchen);
    Serial.println("[EEPROM] Loading kitchen from EEPROM");
    turnRoomOn(g_animationCode, NUM_LIVING_ROOM, NUM_LIVING_ROOM + NUM_KITCHEN, g_colorKitchen);
    
  } else {
    Serial.println("[EEPROM] Could not load from EEPROM");
  }
}

void updateLEDColors(String hexCode, int start, int end, CRGB* color) {
  uint32_t colorValue = (uint32_t) strtol(&hexCode[1], NULL, 16);  // Convert hex string to 32-bit integer
  CRGB calculated_color = CRGB((colorValue >> 16) & 0xFF, (colorValue >> 8) & 0xFF, colorValue & 0xFF);  // Extract RGB components
  *color = calculated_color;
}

void turnRoomOn(int animation, int start, int end, CRGB color) {
  switch(animation) {
    case ANIMATION_STATIC:
      Serial.println("[ANIMATION] STATIC");
      for(int i = start; i < end; i++) {
        leds_room[i] = color;
        FastLED.show();
      }

      break;
    case ANIMATION_INCREMENT:
      Serial.println("[ANIMATION] INCREMENT");
      for(int i = start; i < end; i++) {
        leds_room[i] = color;
        FastLED.show();
        delay(200);
      }

      break;
    case ANIMATION_LIGHT_UP:
      Serial.println("[ANIMATION] LIGHT_UP");
      for(int i = start; i < end; i++) {
        leds_room[i] = color;
      }

      //Changes brightness for everything, other pins would fix this or better code.
      for(int brightness = 0; brightness <= 255; brightness++) {
        FastLED.setBrightness(brightness);
        FastLED.show();
        delay(100);
      }

      break;
    case ANIMATION_REVERSE_INCREMENT:
      Serial.println("[ANIMATION] REVERSE_INCREMENT");
      for(int i = end; i > start; i--) {
        leds_room[i] = color;
        FastLED.show();
        delay(200);
      }

      break;
    default:
      Serial.println("[ANIMATION] No animation set: " + String(animation));
      break;
  }
}

void turnRoomOff(int animation, int start, int end) {
  switch(animation) {
    case ANIMATION_STATIC:
      Serial.println("[ANIMATION] STATIC");
      for(int i = start; i < end; i++) {
        leds_room[i] = CRGB::Black;
        FastLED.show();
      }

      break;
    case ANIMATION_INCREMENT:
      Serial.println("[ANIMATION] INCREMENT");
      for(int i = start; i < end; i++) {
        leds_room[i] = CRGB::Black;
        FastLED.show();
        delay(200);
      }

      break;
    case ANIMATION_LIGHT_UP:
      Serial.println("[ANIMATION] LIGHT_UP");
      for(int i = start; i < end; i++) {
        leds_room[i] = CRGB::Black;
      }

      //Changes brightness for everything, other pins would fix this or better code.
      for(int brightness = 0; brightness <= 255; brightness++) {
        FastLED.setBrightness(brightness);
        FastLED.show();
        delay(100);
      }

      break;
    case ANIMATION_REVERSE_INCREMENT:
      Serial.println("[ANIMATION] REVERSE_INCREMENT");
      for(int i = end; i > start; i--) {
        leds_room[i] = CRGB::Black;
        FastLED.show();
        delay(200);
      }

      break;
    default:
      Serial.println("[ANIMATION] No animation set: " + String(animation));
      break;
  }
}