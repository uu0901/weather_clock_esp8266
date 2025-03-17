#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>

void SerialInit(uint32_t BaudRate = 115200);
void SerialPrint(String message);
#endif