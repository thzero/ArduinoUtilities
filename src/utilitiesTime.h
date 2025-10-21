#ifndef _UTILITIES_TIME_H
#define _UTILITIES_TIME_H

#include <Arduino.h>

#include "communicationSerial.h"

// extern char* convertTime(unsigned long epochS);
extern unsigned long rtcGetEpoch();
extern void rtcInit();
extern void rtcPrintTime();
extern void rtcPrintTimeDigits(int digits);
extern void rtcTimestampCommand(uint8_t* commandBuffer, uint16_t commandBufferLength);
extern void rtcTimestampCommandSend();
extern void rtcTimestampCommandSend(CommunicationSerial* serial);
extern void rtcTimestampCommandSend(unsigned long timestamp);
extern void rtcSetTime(unsigned long time);

#endif