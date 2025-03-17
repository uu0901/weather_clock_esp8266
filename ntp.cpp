#include "ntp.h"
#include "Serial.h"
#include "oled.h"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp.aliyun.com", 8 * 3600);
void NtpInit()
{
  SerialPrint("初始化NTP客户端...");
  timeClient.begin();
  SerialPrint("初始化NTP客户端完成");
}
String NtpGetTime()
{
  timeClient.update();
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds);
  return String(timeStr);
}
String NtpGetDate()
{
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int year = ptm->tm_year + 1900;
  int month = ptm->tm_mon + 1;
  int day = ptm->tm_mday;
  char dateStr[9];
  sprintf(dateStr, "%02d-%02d-%02d", year, month, day);
  return String(dateStr);
}
bool NtpSync()
{
  SerialPrint("正在同步网络时间...");
  OledShow(0, 16, "正在同步网络时间...");
  if(timeClient.forceUpdate())
  {
    SerialPrint("时间同步成功");
    OledClear();
    OledShow(0, 16, "时间同步成功");
    OledShow(0, 32, "日期" + NtpGetDate());
    OledShow(0, 48, "时间" + NtpGetTime());
    SerialPrint("日期" + NtpGetDate());
    SerialPrint("时间" + NtpGetTime());
    return true;
  }
  else
  {
    SerialPrint("时间同步失败");
    OledClear();
    OledShow(0, 16, "时间同步失败");
    return false;
  }
}