#ifndef _TIME_H
#define _TIME_H

#if defined(ESP32)

#include <Arduino.h>

extern char* convertTime(unsigned long epochS);
extern unsigned long rtcGetEpoch();
extern void rtcPrintTime();
extern void rtcTimestampCommand(uint8_t commandBuffer[], int commandBufferLength);
extern void rtcTimestampCommandSend(unsigned long timestamp);
extern void rtcSetTime(unsigned long time);

#endif

#endif