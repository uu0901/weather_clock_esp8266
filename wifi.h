#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID          "ALIENTEK-JS"
#define WIFI_PASSWORD      "15902020353"

void WifiInit();
bool WifiConnect(const char *ssid =  WIFI_SSID, const char *password = WIFI_PASSWORD);
#endif