#include "weather.h"
#include "serial.h"
WeatherInfo CurrentWeather;
void WeatherInit()
{
  SerialPrint("初始化天气模块...");
  CurrentWeather.city = "未知";

  CurrentWeather.today_day_weather = "未知";
  CurrentWeather.today_night_weather = "未知";
  CurrentWeather.today_rainfall = "0";
  CurrentWeather.today_precip_rainfall = "0";
  CurrentWeather.today_wind_direction = "未知";
  CurrentWeather.today_wind_direction_degree = "0";
  CurrentWeather.today_wind_speed = "0";
  CurrentWeather.today_wind_scale = "0";
  CurrentWeather.today_high_temp = "0";
  CurrentWeather.today_low_temp = "0";
  CurrentWeather.today_humidity = "0";

  CurrentWeather.tomorrow_day_weather = "未知";
  CurrentWeather.tomorrow_night_weather = "未知";
  CurrentWeather.tomorrow_rainfall = "0";
  CurrentWeather.tomorrow_precip_rainfall = "0";
  CurrentWeather.tomorrow_wind_direction = "未知";
  CurrentWeather.tomorrow_wind_direction_degree = "0";
  CurrentWeather.tomorrow_wind_speed = "0";
  CurrentWeather.tomorrow_wind_scale = "0";
  CurrentWeather.tomorrow_high_temp = "0";
  CurrentWeather.tomorrow_low_temp = "0";
  CurrentWeather.tomorrow_humidity = "0";
  SerialPrint("初始化天气模块完成");
}
bool WeatherUpdate()
{
  SerialPrint("正在请求天气信息...");
  char url[200];
  sprintf(url, WEATHER_URL, WEATHER_API_KEY, WEATHER_CITY);
  SerialPrint("请求URL：" + String(url));
  WiFiClient client;
  HTTPClient http;
  if(http.begin(client, url))
  {
    SerialPrint("开始HTTP请求...");
    int HttpCode = http.GET();
    SerialPrint("HTTP请求码：" + String(HttpCode));
    if(HttpCode == HTTP_CODE_OK)
    {
        String PayLoad = http.getString();
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, PayLoad);
        if(!error)
        {
            JsonObject results_0 = doc["results"][0];
            JsonObject location = results_0["location"];
            JsonArray daily = results_0["daily"];
            JsonObject today = daily[0];
            JsonObject tomorrow = daily[1];
            if(!today.isNull())
            {
              CurrentWeather.city = location["name"].as<String>();
              
              CurrentWeather.today_day_weather = today["text_day"].as<String>();
              CurrentWeather.today_night_weather = today["text_night"].as<String>();
              CurrentWeather.today_rainfall = today["rainfall"].as<String>();
              CurrentWeather.today_precip_rainfall = today["precip"].as<String>();
              CurrentWeather.today_wind_direction = today["wind_direction"].as<String>();
              CurrentWeather.today_wind_direction_degree = today["wind_direction_degree"].as<String>();
              CurrentWeather.today_wind_speed = today["wind_speed"].as<String>();
              CurrentWeather.today_wind_scale = today["wind_scale"].as<String>();
              CurrentWeather.today_high_temp = today["high"].as<String>();
              CurrentWeather.today_low_temp = today["low"].as<String>();
              CurrentWeather.today_humidity = today["humidity"].as<String>();

              CurrentWeather.tomorrow_day_weather = tomorrow["text_day"].as<String>();
              CurrentWeather.tomorrow_night_weather = tomorrow["text_night"].as<String>();
              CurrentWeather.tomorrow_rainfall = tomorrow["rainfall"].as<String>();
              CurrentWeather.tomorrow_precip_rainfall = tomorrow["precip"].as<String>();
              CurrentWeather.tomorrow_wind_direction = tomorrow["wind_direction"].as<String>();
              CurrentWeather.tomorrow_wind_direction_degree = tomorrow["wind_direction_degree"].as<String>();
              CurrentWeather.tomorrow_wind_speed = tomorrow["wind_speed"].as<String>();
              CurrentWeather.tomorrow_wind_scale = tomorrow["wind_scale"].as<String>();
              CurrentWeather.tomorrow_high_temp = tomorrow["high"].as<String>();
              CurrentWeather.tomorrow_low_temp = tomorrow["low"].as<String>();
              CurrentWeather.tomorrow_humidity = tomorrow["humidity"].as<String>();

              SerialPrint("城市：" + CurrentWeather.city);
              SerialPrint("");
              SerialPrint("今日 白天天气：" +   CurrentWeather.today_day_weather);
              SerialPrint("     夜晚天气：" +   CurrentWeather.today_night_weather);
              SerialPrint("     降雨量：" +     CurrentWeather.today_rainfall);
              SerialPrint("     预期降雨量：" +  CurrentWeather.today_precip_rainfall);
              SerialPrint("     风向：" +       CurrentWeather.today_wind_direction);
              SerialPrint("     风向角度：" +    CurrentWeather.today_wind_direction_degree);
              SerialPrint("     风速：" +        CurrentWeather.today_wind_speed);
              SerialPrint("     风力等级：" +    CurrentWeather.today_wind_scale);
              SerialPrint("     最高温度：" +   CurrentWeather.today_high_temp);
              SerialPrint("     最低温度：" +   CurrentWeather.today_low_temp);
              SerialPrint("     湿度：" +       CurrentWeather.today_humidity);
              SerialPrint("");
              SerialPrint("明日 白天天气：" +   CurrentWeather.tomorrow_day_weather);
              SerialPrint("     夜晚天气：" +   CurrentWeather.tomorrow_night_weather);
              SerialPrint("     降雨量：" +     CurrentWeather.tomorrow_rainfall);
              SerialPrint("     预期降雨量：" +  CurrentWeather.tomorrow_precip_rainfall);
              SerialPrint("     风向：" +       CurrentWeather.tomorrow_wind_direction);
              SerialPrint("     风向角度：" +    CurrentWeather.tomorrow_wind_direction_degree);
              SerialPrint("     风速：" +        CurrentWeather.tomorrow_wind_speed);
              SerialPrint("     风力等级：" +    CurrentWeather.tomorrow_wind_scale);
              SerialPrint("     最高温度：" +   CurrentWeather.tomorrow_high_temp);
              SerialPrint("     最低温度：" +   CurrentWeather.tomorrow_low_temp);
              SerialPrint("     湿度：" +       CurrentWeather.tomorrow_humidity);
              http.end();
              return true;
            }
            else
            {
              SerialPrint("JSON解析失败，找不到天气数据");
            }
        }
        else
        {
            SerialPrint("JSON解析错误：" + String(error.c_str()));
        }
    }
    http.end();
  }
  SerialPrint("天气更新失败");
  return false;
}

WeatherInfo WeatherGetInfo()
{
  return CurrentWeather;
}