#include "serial.h"

void SerialInit(uint32_t BaudRate)
{
  Serial.begin(BaudRate);
}
void SerialPrint(String message)
{
  Serial.print("[");
  Serial.print(millis());
  Serial.print("]");
  Serial.println(message);
}