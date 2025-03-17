#include "wifi.h"
#include "serial.h"
void WifiInit()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  SerialPrint("WiFi模块初始化完成");
}
bool WifiConnect(const char *ssid , const char *password)
{
  SerialPrint("正在连接WiFi");
  WiFi.begin(ssid, password);

  int temp = 0;
  while(WiFi.status() != WL_CONNECTED  && temp <= 20)
  {
      delay(1000);
      temp ++;
      SerialPrint("等待连接..." + String(temp + 1));
  }
  if(WiFi.status() == WL_CONNECTED)
  {
      SerialPrint("WiFi连接成功");
      SerialPrint("IP地址：" + WiFi.localIP().toString());
      return true;
  }
  else
  {
      SerialPrint("WiFi连接失败");
      return false;
  }
}