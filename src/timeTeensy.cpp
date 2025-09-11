#if defined(TEENSYDUINO)

#include <TimeLib.h>

#include <communicationSerial.h>
#include "timeTeensy.h"
#include <utilities.h>

char* convertTime(unsigned long epochS) {
  char *buff = (char *)malloc(32 * sizeof(char)); 
  if (buff == NULL)
        return NULL; // Handle allocation failure
  sprintf(buff, "%02d.%02d.%02d %02d:%02d:%02d", month(epochS), day(epochS), year(epochS), hour(epochS), minute(epochS), second(epochS));
  return buff;
}

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
  rtcPrintTimeDigits(hour());
  Serial.print(F(":"));
  rtcPrintTimeDigits(minute());
  Serial.print(F(":"));
  rtcPrintTimeDigits(second());
  Serial.print(F(" "));
  Serial.print(day());
  Serial.print(F(" "));
  Serial.print(month());
  Serial.print(F(" "));
  Serial.print(year()); 
  Serial.println(); 
}

void rtcPrintTimeDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  if (digits < 10)
    Serial.print(F("0"));
  Serial.print(digits);
}

void rtcTimestampCommand(uint8_t commandBuffer[], int commandBufferLength) {
  Serial.print(F("rtcTimestampCommand... commandBufferLength="));
  Serial.println(commandBufferLength);

  int size = (int)sizeof(unsigned long) + 1;
  Serial.print(F("rtcTimestampCommand... size="));
  Serial.println(size);
  if ((commandBufferLength < size) || (commandBufferLength > size)) {
    Serial.printf(F("Invalid timestamp; requires %d character UNIX timestamp."), size);
    return;
  }

  unsigned long epoch = convertUnsignedByteArrayToUnsignedLong(&commandBuffer[1]);
  Serial.print(F("rtcTimestampCommand... epoch="));
  Serial.println(epoch);

  setTime(epoch);

//   Serial.print(F("Current time is: "));
//   rtcPrintTime();
}

void rtcTimestampCommandSend() {
  size_t size = sizeof(unsigned long) + 2;
  uint8_t buffer[size];
  memset(buffer, 0, size);
  buffer[0] = '$';
  buffer[size - 1] = ';';
  unsigned long epoch = rtcGetEpoch();

  // Serial.println();
  // Serial.printf(F("Epoch: %ul\n"), epoch);

  convertUnsignedLongToUnsignedByteArray(epoch, &buffer[1]); // pointer to the second byte to start epoch...

  // convertPrintUnsignedByteArrayUnsignedLong(&buffer[1]);
  
  // unsigned long epoch2 = convertUnsignedByteArrayToUnsignedLong(&buffer[1]);
  // Serial.printf(F("Epoch2: %ul\n"), epoch2);
  // Serial.printf(F("Epoch == Epoch2: %ul=%ul %d\n"), epoch, epoch2, epoch == epoch2);

  Serial.printf(F("Sending epoch '%d' as bytes...\n"), epoch);
// #ifdef DEBUG_SERIAL2
    Serial.print(F("Sent: "));
    for (size_t i = 0; i < size; i++)
      Serial.printf("%d", buffer[i]);
    Serial.println();
// #endif
  // Serial2.write(buffer, size);
  communicationQueue(buffer, size);
  Serial.println(F("...sent"));
}

void rtcSetTime(unsigned long time) {
  setTime(time);
}

#endif