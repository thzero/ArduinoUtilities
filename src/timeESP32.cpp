#if defined(ESP32)

#include <TimeLib.h>
#include <ESP32Time.h>

#include "communicationSerial.h"
#include "timeESP32.h"
#include <utilities.h>

ESP32Time rtc;

char* convertTime(unsigned long epochS) {
    char buff[32];
    sprintf(buff, "%02d.%02d.%02d %02d:%02d:%02d", month(epochS), day(epochS), year(epochS), hour(epochS), minute(epochS), second(epochS));
    return buff;
}

unsigned long rtcGetEpoch() {
    return rtc.getEpoch();
}

void rtcPrintTime() {
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
}

void rtcTimestampCommand(uint8_t commandBuffer[], int commandBufferLength) {
//   Serial.print(F("rtcTimestampCommand... commandBufferLength="));
//   Serial.println(commandBufferLength);

  size_t size = sizeof(unsigned long) + 1;
//   Serial.print(F("rtcTimestampCommand... size="));
//   Serial.println(size);
  if ((commandBufferLength < size) || (commandBufferLength > size)) {
    Serial.printf(F("Invalid timestamp; requires %d character UNIX timestamp."), size);
    return;
  }

  unsigned long epoch = convertUnsignedByteArrayToUnsignedLong(&commandBuffer[1]);
  Serial.printf(F("rtcTimestampCommand... setting '%d' as the epoch.\n"), epoch);
  Serial.println(epoch);

  rtcSetTime(epoch);

  Serial.print(F("Current time is: "));
  rtcPrintTime();
}

void rtcTimestampCommandSend(unsigned long timestamp) {
  rtcSetTime(timestamp);

  size_t size = sizeof(unsigned long) + 2;
  uint8_t buffer[size];
  memset(buffer, 0, size);
  buffer[0] = '$';
  buffer[size - 1] = ';';

  Serial.println();
  Serial.printf(F("Epoch: %ul\n"), timestamp);

  convertUnsignedLongToUnsignedByteArray(timestamp, &buffer[1]); // pointer to the second byte to start epoch...

  convertPrintUnsignedByteArrayUnsignedLong(&buffer[1]);
  
  unsigned long epoch2 = convertUnsignedByteArrayToUnsignedLong(&buffer[1]);
  Serial.printf(F("Epoch2: %ul\n"), epoch2);
  Serial.printf(F("Epoch == Epoch2: %ul=%ul %d\n"), timestamp, epoch2, timestamp == epoch2);

  Serial.print(F("Sending epoch bytes..."));
  for (int i = 0; i < size; i++)
      Serial.print(buffer[i]);
  Serial.println();
  // Serial2.write(buffer, size);
  communicationQueue(buffer, size);
  Serial.println(F("...sent"));
}

void rtcSetTime(unsigned long time) {
    rtc.setTime(time);
}

#endif