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
CRGB g_LEDColor = CRGB::BlueViolet;

int room = 0;

String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATUREC"){
    return "test";
  }
  else if(var == "TEMPERATUREF"){
    return "test";
  }
  return String();
}

//Prototypes
void turnRoomOn();
void turnRoomOff();
void updateLEDColors(String hexCode);

//ROOM_PORT: 4
void setup() {
  system_update_cpu_freq(SYS_CPU_160MHZ);
  Serial.begin(9600);

  //Add temperature sensor
  dht.begin();

  FastLED.addLeds<LED_TYPE, ROOM_PORT, COLOR_ORDER>(leds_room, NUM_ROOM).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

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

  server.on("/roomOn", HTTP_GET, [](AsyncWebServerRequest *request) {
    turnRoomOn();
    Serial.println("Turning Room On");
    //request->send_P(200, "text/plain", "Room On", processor);
  });

  server.on("/roomOff", HTTP_GET, [](AsyncWebServerRequest *request) {
    turnRoomOff();
    Serial.println("Turning Room Off");
    //request->send_P(200, "text/plain", "Room On", processor);
  });

  server.on("/setColor", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("color")) {
      String hexCode = request->getParam("color")->value();
      Serial.println("Setting Color: " + hexCode);
      updateLEDColors(hexCode);
    }
  });

  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature() - 5;

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\n");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print("\n\n");

    if (isnan(t) || isnan(h)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }

}

void updateLEDColors(String hexCode) {
  if(leds_room[1] != CRGB::Black) {
    Serial.println("LEDS are ON chaning color to " + hexCode);
    uint32_t colorValue = (uint32_t) strtol(&hexCode[1], NULL, 16);  // Convert hex string to 32-bit integer
    CRGB color = CRGB((colorValue >> 16) & 0xFF, (colorValue >> 8) & 0xFF, colorValue & 0xFF);  // Extract RGB components
    g_LEDColor = color;

    Serial.println("LEDS START");
    for(int i = 0; i < NUM_ROOM; i++) {
      leds_room[i] = color;
      FastLED.show();
    }
    
    Serial.println("LEDS END");
  }
}

void turnRoomOn() {
  for(int i = 0; i < NUM_ROOM; i++) {
    leds_room[i] = g_LEDColor;
    delay(10);
    FastLED.show();
  }
}

void turnRoomOff() {
  for(int i = 0; i < NUM_ROOM; i++) {
    leds_room[i] = CRGB::Black;
    delay(10);
    FastLED.show();
  }
}