#pragma once

#include <Arduino.h>
#include <DHT.h>

#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
    #include <ESPAsyncWebServer.h>
#endif

#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <FastLED.h>
//#include <ESP8266WebServer.h>  

#include "headers/defs.h"
#include "../src/src/wifi.h"
#include "../src/src/webpage.h"
