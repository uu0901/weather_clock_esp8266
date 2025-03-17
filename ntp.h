#ifndef NTP_H
#define NTP_H

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
void NtpInit();
String NtpGetTime();
String NtpGetDate();
bool NtpSync();
#endif