#include "headers/includes.h"

CRGB leds_room[NUM_ROOM];

const char* ssid = "cablelink_0823061";
const char* password = "kkYo57icS5(65ZYU";

//Set webserver to port 80
ESP8266WebServer server(80);

String header;

String outputRoomState = "off";

void handleRoot();
void handleRoomOn();
void handleRoomOff();
void handleNotFound();

int room = 0;

//ROOM_PORT: 4
void setup() {
  system_update_cpu_freq(SYS_CPU_160MHZ);
  Serial.begin(57600);
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
  
  server.on("/", handleRoot);
  server.on("/roomOn", HTTP_POST, handleRoomOn);
  server.on("/roomOff", HTTP_POST, handleRoomOff);

  server.onNotFound(handleNotFound);
  
  server.begin();
}

void loop() {
  server.handleClient();

  if(room == 0) {

  }

  if(room == 1) {

  }
}

void handleRoot() {
  server.send(200, "text/html", "<style>body {background: black}</style>blue<form action=\"/roomOn\" method=\"POST\"><input type=\"submit\" value=\"Room On\"></form> <form action=\"/roomOff\" method=\"POST\"><input type=\"submit\" value=\"Room Off\"></form>");
}

void showLED() {
  
}

void turnOffLED() {

}

void handleRoomOn() {
  for(int i = 0; i < NUM_ROOM; i++) {
    leds_room[i] = CRGB::DarkBlue;
    delay(10);
    FastLED.show();
    yield();
  }

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleRoomOff() {
  for(int i = 0; i < NUM_ROOM; i++) {
    leds_room[i] = CRGB::Black;
    delay(10);
    FastLED.show();
    yield();
  }

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleNotFound() {

}