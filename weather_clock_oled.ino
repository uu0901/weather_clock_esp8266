#include "serial.h"
#include "wifi.h"
#include "oled.h"
#include "ntp.h"
#include "weather.h"

#define BUTTON_PIN 0   // ESP8266的GPIO0引脚（对应Arduino D3，依开发板而定）
#define DEBOUNCE_TIME 50    // 消抖时间(ms)
#define LONG_PRESS_TIME 1000 // 长按判定时间(ms)

#include <Ticker.h>
Ticker keyTicker;
volatile bool buttonPressed = false;
volatile unsigned long pressStartTime = 0;

// 按键状态枚举
enum ButtonState {
  IDLE,
  PRESSED,
  LONG_PRESSED
};
enum Mode {
  none,
  today,
  tomorrow
};
volatile ButtonState buttonState = IDLE;

void checkButton() {
  static int lastState = HIGH;
  int currentState = digitalRead(BUTTON_PIN);

  // 状态变化检测
if (currentState != lastState) 
{
    delay(DEBOUNCE_TIME); // 消抖延时
    currentState = digitalRead(BUTTON_PIN);
    
    if (currentState == LOW && lastState == HIGH) {
      pressStartTime = millis();
      buttonState = PRESSED;
    } 
    else if (currentState == HIGH && lastState == LOW) {
      if (buttonState == PRESSED) {
        // 短按处理
        Serial.println("[短按] 检测到按键按下");
        buttonPressed = true;
      }
      buttonState = IDLE;
    }
    lastState = currentState;
  } 
    // 长按检测
  if (buttonState == PRESSED && (millis() - pressStartTime) > LONG_PRESS_TIME) {
    buttonState = LONG_PRESSED;
    Serial.println("[长按] 检测到长按操作");
    buttonPressed = true;
  }
}

void ShowAllInfo()
{

}
bool SyncNetworkServices()
{
  if(WifiConnect())
  {
    OledClear();
    OledShow(0, 16, "WiFi连接成功");
    OledShow(0, 32, "SSID: " + String(WIFI_SSID));
    OledShow(0, 48, "IP: " + WiFi.localIP().toString());
    delay(1000);
  }
  else
  {
    OledClear();
    OledShow(0, 16, "WiFi连接失败");
    OledShow(0, 32, "SSID: " + String(WIFI_SSID));
    return false;
  }
  OledClear();
  OledShow(0, 16, "正在同步网络时间");
  if(!NtpSync())
  {
    SerialPrint("NTP同步失败，请检查网络连接");
  }
  delay(1000);
  OledClear();
  OledShow(0, 16, "正在获取天气信息");
  if(!WeatherUpdate())
  {
    SerialPrint("天气信息获取失败");
  }
  delay(1000);
  return true;
}
void setup() {
  // put your setup code here, to run once:
  SerialInit();
  SerialPrint("系统启动");

  OledInit();
  OledShow(0, 16, "系统启动中...");
  
  OledShow(0, 32, "正在初始化WiFi...");
  WifiInit();

  NtpInit();
  WeatherInit();

  OledShow(0, 48, "正在连接WiFi...");
  OledShow(0, 64, "SSID: " + String(WIFI_SSID));
  SyncNetworkServices();


  pinMode(BUTTON_PIN, INPUT_PULLUP); // 启用内部上拉电阻[[7,12]]
  keyTicker.attach_ms(10, checkButton); // 每10ms检测一次按键状态[[12]]
  Serial.println("系统启动完成，等待按键操作...");
}

uint8_t shorttimes = 0;
uint8_t longtimes = 0;
uint8_t mode = today;
void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() == WL_CONNECTED)
  {
    static uint32_t LastWeatherUpdate = 0;
    static uint32_t LastDisplayUpdate = 0;
    uint32_t CurrentMillis = millis();
    if(CurrentMillis - LastWeatherUpdate >= 600000)
    {
      if(WeatherUpdate())
      {
        // ShowAllInfo();
      }
      LastWeatherUpdate = CurrentMillis;
    }
    if(CurrentMillis - LastDisplayUpdate >= 1000)
    {
      switch(mode)
      {
        case today:
        {
          switch(shorttimes)
          {
            case 0 : 
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("今日 最高温度 " + WeatherGetInfo().today_high_temp + "℃");
              u8g2.setCursor(2+0, 55);
              u8g2.print("     最低温度 " + WeatherGetInfo().today_low_temp + "℃"); 
              u8g2.sendBuffer();
            }
            break;
            case 1: 
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("今日 白天天气 " + WeatherGetInfo().today_day_weather);
              u8g2.setCursor(2+0, 55);
              u8g2.print("     夜晚温度 " + WeatherGetInfo().today_night_weather); 
              u8g2.sendBuffer();
            }
            break;
            case 2:
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("今日 降雨量 " + WeatherGetInfo().today_rainfall);
              u8g2.setCursor(2+0, 55);
              u8g2.print("     预期降雨量 " + WeatherGetInfo().today_precip_rainfall); 
              u8g2.sendBuffer();
            }
            break;
            case 3:
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("今日 风向 " + WeatherGetInfo().today_wind_direction);
              u8g2.setCursor(2+0, 55);
              u8g2.print("     风向角度 " + WeatherGetInfo().today_wind_direction_degree); 
              u8g2.sendBuffer();
            }
            break;
            case 4:
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("今日 风速 " + WeatherGetInfo().today_wind_speed);
              u8g2.setCursor(2+0, 55);
              u8g2.print("     风力等级 " + WeatherGetInfo().today_wind_scale); 
              u8g2.sendBuffer();
            }
            break;
            case 5:
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("今日 湿度 " + WeatherGetInfo().today_humidity);
              u8g2.setCursor(2+0, 55);
              u8g2.print("     城市 " + WeatherGetInfo().city); 
              u8g2.sendBuffer();
            }
            break;
          }
        }
        break;

        case tomorrow:
        {
          switch(shorttimes)
          {
            case 0 : 
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("明日 最高温度 " + WeatherGetInfo().tomorrow_high_temp + "℃");
              u8g2.setCursor(2+0, 55);
              u8g2.print("     最低温度 " + WeatherGetInfo().tomorrow_low_temp + "℃"); 
              u8g2.sendBuffer();
            }
            break;
            case 1: 
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("明日 白天天气 " + WeatherGetInfo().tomorrow_day_weather);
              u8g2.setCursor(2+0, 55);
              u8g2.print("     夜晚温度 " + WeatherGetInfo().tomorrow_night_weather); 
              u8g2.sendBuffer();
            }
            break;
            case 2:
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("明日 降雨量 " + WeatherGetInfo().tomorrow_rainfall);
              u8g2.setCursor(2+0, 55);
              u8g2.print("     预期降雨量 " + WeatherGetInfo().tomorrow_precip_rainfall); 
              u8g2.sendBuffer();
            }
            break;
            case 3:
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("明日 风向 " + WeatherGetInfo().tomorrow_wind_direction);
              u8g2.setCursor(2+0, 55);
              u8g2.print("     风向角度 " + WeatherGetInfo().tomorrow_wind_direction_degree); 
              u8g2.sendBuffer();
            }
            break;
            case 4:
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("明日 风速 " + WeatherGetInfo().tomorrow_wind_speed);
              u8g2.setCursor(2+0, 55);
              u8g2.print("     风力等级 " + WeatherGetInfo().tomorrow_wind_scale); 
              u8g2.sendBuffer();
            }
            break;
            case 5:
            {
              u8g2.clearBuffer();
              u8g2.setCursor(2 + 0, 16);
              u8g2.print(NtpGetDate() + " " + NtpGetTime());
              u8g2.setCursor(2 + 0, 35);
              u8g2.print("明日 湿度 " + WeatherGetInfo().tomorrow_humidity);
              u8g2.setCursor(2+0, 55);
              u8g2.print("     城市 " + WeatherGetInfo().city); 
              u8g2.sendBuffer();
            }
            break;
          }
        }
        break;
      }
        
      LastDisplayUpdate = CurrentMillis;
    }
  }

    if (buttonPressed) 
    {
    // 执行按键响应操作（示例：切换LED状态）
      if (buttonState == LONG_PRESSED)
      {
        Serial.println("执行长按对应操作");
        longtimes ++;
        shorttimes = 0;
        if(longtimes == 2) longtimes = 0;

        Serial.println("长按次数");
        Serial.println(longtimes);
        OledClear();
      } 
      else 
      {
        Serial.println("执行短按对应操作");
        shorttimes ++;
        if(shorttimes > 5) shorttimes = 0;
        Serial.println("短按次数");
        Serial.println(shorttimes);
        OledClear();
      }
      buttonPressed = false;
      if(longtimes == 0) mode = today;
      else if(longtimes == 1) mode = tomorrow;
    }


}
