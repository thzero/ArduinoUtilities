#if defined(ESP32)
#include <ESP32Time.h>
#include <TimeLib.h>
#elif defined(TEENSYDUINO)
#include <TimeLib.h>
#endif

#include "communicationConstants.h"
#include "communicationSerial.h"
#include <utilities.h>
#include <utilitiesTime.h>

// char* convertTime(unsigned long epochS) {
//     char buff[32];
//     sprintf(buff, "%02d.%02d.%02d %02d:%02d:%02d", month(epochS), day(epochS), year(epochS), hour(epochS), minute(epochS), second(epochS));
//     return buff;
// }

void rtcPrintTimeDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  if (digits < 10)
    Serial.print(F("0"));
  Serial.print(digits);
}

#if defined(ESP32)
ESP32Time rtc;

unsigned long rtcGetEpoch() {
    return rtc.getEpoch();
}

void rtcInit() {
}

void rtcPrintTime() {
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
}

void rtcTimestampCommand(uint8_t* commandBuffer, uint16_t commandBufferLength) {
//   Serial.print(F("rtcTimestampCommand... commandBufferLength="));
//   Serial.println(commandBufferLength);

//   size_t size = sizeof(unsigned long) + 1;
// //   Serial.print(F("rtcTimestampCommand... size="));
// //   Serial.println(size);
//   if ((commandBufferLength < size) || (commandBufferLength > size)) {
//     Serial.printf(F("Invalid timestamp; requires %d character UNIX timestamp."), size);
//     return;
//   }

//   unsigned long epoch = convertUnsignedByteArrayToUnsignedLong(&commandBuffer[1]);
//   Serial.printf(F("rtcTimestampCommand... setting '%d' as the epoch.\n"), epoch);
//   Serial.println(epoch);

//   rtcSetTime(epoch);

//   Serial.print(F("Current time is: "));
//   rtcPrintTime();
  size_t size = sizeof(unsigned long);
//   Serial.print(F("rtcTimestampCommand... size="));
//   Serial.println(size);
  if ((commandBufferLength < size) || (commandBufferLength > size)) {
    Serial.printf(F("Invalid timestamp; requires %d character UNIX timestamp."), size);
    return;
  }

  unsigned long epoch = convertUnsignedByteArrayToUnsignedLong(&commandBuffer[0]);
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

  Serial.printf(F("Sending epoch '%d' as bytes...\n"), timestamp);
  Serial.print(F("Sent: "));
  for (int i = 0; i < size; i++)
      Serial.print(buffer[i]);
  Serial.println();
  // Serial2.write(buffer, size);
  // communicationSerialQueue(buffer, size);
  // _communicationSerialObj.queue(buffer, size);
  Serial.println(F("...sent"));
}

void rtcTimestampCommandSend() {
  rtcTimestampCommandSend(rtcGetEpoch());
}

void rtcSetTime(unsigned long time) {
    rtc.setTime(time);
}
#endif

#if defined(TEENSYDUINO)
unsigned long rtcGetEpoch() {
  return now();
}

// void rtcGetTime() {
  // time_t time = rtc_get();
//   // Serial.println(RTC.getTime("%A, %B %d %Y %H:%M:%S"));
// }

time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

void rtcInit() {
  setSyncProvider(getTeensy3Time);   // the function to get the time from the RTC
  bool failed = false;
  if (timeStatus() == timeNotSet) {
    Serial.println(F("Unable to sync with the RTC; time not set."));
    failed = true;
  }
  else if (timeStatus() == timeNeedsSync) {
    Serial.println(F("Unable to sync with the RTC; time needs sync."));
    failed = true;
  }
  else if (timeStatus() != timeSet) {
    Serial.println(F("Unable to sync with the RTC; time was not set."));
    failed = true;
  }
  else
    Serial.println(F("RTC has set the system time"));

  if (failed) {
    setTime(1735689600);
  }
  rtcPrintTime();
}

void rtcPrintTime() {
  Serial.print(month());
  Serial.print(F("/"));
  Serial.print(day());
  Serial.print(F("/"));
  Serial.print(year()); 
  Serial.print(F(" "));
  rtcPrintTimeDigits(hour());
  Serial.print(F(":"));
  rtcPrintTimeDigits(minute());
  Serial.print(F(":"));
  rtcPrintTimeDigits(second());
  Serial.println(); 
}

void rtcTimestampCommand(uint8_t* commandBuffer, uint16_t commandBufferLength) {
  Serial.print(F("rtcTimestampCommand... commandBufferLength="));
  Serial.println(commandBufferLength);

  size_t size = sizeof(unsigned long) + 1;
  Serial.print(F("rtcTimestampCommand... size="));
  Serial.println(size);
  if ((commandBufferLength < size) || (commandBufferLength > size)) {
    Serial.printf(F("Invalid timestamp; requires %d character UNIX timestamp."), size);
    return;
  }

  Serial.println("communication-serial-queue: trying to queue.");
  Serial.println("communication-serial-queue: requested bytes: ");
  for (size_t i = 0; i < size; i++)
      Serial.printf("%d ", commandBuffer[i]);
  Serial.println();

  unsigned long epoch = convertUnsignedByteArrayToUnsignedLong(commandBuffer);
  Serial.print(F("rtcTimestampCommand... epoch="));
  Serial.println(epoch);

  setTime(epoch);

//   Serial.print(F("Current time is: "));
//   rtcPrintTime();
}

void rtcTimestampCommandSend() {
  // size_t size = sizeof(unsigned long) + 2;
  // uint8_t buffer[size];
  // memset(buffer, 0, size);
  // buffer[0] = '$';
  // buffer[size - 1] = ';';
  size_t size = sizeof(unsigned long);
  uint8_t buffer[size];
  memset(buffer, 0, size);
  unsigned long epoch = rtcGetEpoch();

  // Serial.println();
  // Serial.printf(F("Epoch: %ul\n"), epoch);

  convertUnsignedLongToUnsignedByteArray(epoch, buffer); // pointer to the second byte to start epoch...

  // convertPrintUnsignedByteArrayUnsignedLong(&buffer[1]);
  
  // unsigned long epoch2 = convertUnsignedByteArrayToUnsignedLong(&buffer[1]);
  // Serial.printf(F("Epoch2: %ul\n"), epoch2);
  // Serial.printf(F("Epoch == Epoch2: %ul=%ul %d\n"), epoch, epoch2, epoch == epoch2);

  Serial.printf(F("Sending epoch '%d' as bytes...\n"), epoch);
// #ifdef DEBUG_SERIAL2
    Serial.print(F("Sent: "));
    for (size_t i = 0; i < size; i++)
      Serial.printf("%d ", buffer[i]);
    Serial.println();
// #endif
  // Serial2.write(buffer, size);
  // communicationSerialQueue(buffer, size);
  _communicationSerialObj.queue(COMMUNICATION_RTC, buffer, size);
  // _communicationSerialObj.queue(2, buffer, size);
  Serial.println(F("...sent"));
}

void rtcSetTime(unsigned long time) {
  setTime(time);
}
#endif