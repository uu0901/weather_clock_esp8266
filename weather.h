#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
//https://api.seniverse.com/v3/weather/daily.json?key=SyWbZlkKQW3IZ8CcS&location=guangzhou&language=zh-Hans&unit=c&start=0&days=2
#define WEATHER_API_KEY "SyWbZlkKQW3IZ8CcS"
#define WEATHER_CITY    "guangzhou"
#define WEATHER_URL     "http://api.seniverse.com/v3/weather/daily.json?key=%s&location=%s&language=zh-Hans&unit=c&start=0&days=2"
struct WeatherInfo{
  String city;

  String today_day_weather;
  String today_night_weather;
  String today_rainfall; 
  String today_precip_rainfall; 
  String today_wind_direction;   
  String today_wind_direction_degree;   
  String today_wind_speed;   
  String today_wind_scale; 
  String today_high_temp;
  String today_low_temp;
  String today_humidity;

  String tomorrow_day_weather;
  String tomorrow_night_weather;
  String tomorrow_rainfall; 
  String tomorrow_precip_rainfall; 
  String tomorrow_wind_direction;   
  String tomorrow_wind_direction_degree;   
  String tomorrow_wind_speed;   
  String tomorrow_wind_scale; 
  String tomorrow_high_temp;
  String tomorrow_low_temp;
  String tomorrow_humidity;
};
void WeatherInit();
bool WeatherUpdate();
WeatherInfo WeatherGetInfo();
#endif