#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include <debug.h>
#include "fileSystemLFS.h"
#include <utilities.h>

fileSystemLFS::fileSystemLFS() {
}

File fileSystemLFS::openFile(char* path) {
  // Open the root directory
#if defined(ESP32)
  File file = LittleFS.open(path);
#elif defined(TEENSYDUINO)
  File file = File(); // TODO
#endif
  if (!file || file.isDirectory()) {
    Serial.printf(F("...failed to open the %s.\n"), path);
    return File();
  }

  return file;
}

bool fileSystemLFS::setup() {
  Serial.println(F("\tLittleFS begin..."));
#if defined(ESP32)
  bool results = LittleFS.begin(false);
#elif defined(TEENSYDUINO)
  bool results = false;
#endif
  if (!results) {
    Serial.println(F("\tLittleFS mount failed"));
    Serial.println(F("\tDid not find filesystem; starting format"));

    // format if begin fails
    Serial.println(F("\tLittleFS trying to format..."));
#if defined(ESP32)
    bool results = LittleFS.begin(true);
#elif defined(TEENSYDUINO)
    bool results = false;
#endif
  if (!results) {
      Serial.println(F("\tLittleFS format failed"));
      Serial.println(F("\tFormatting not possible"));

      Serial.println(F("...file system setup failed."));
      return false;
    }

    Serial.println(F("Formated"));
  }
  
  Serial.println(F("...file system setup successful."));
  return true;
}

long fileSystemLFS::totalBytes() {
#if defined(ESP32)
  return LittleFS.totalBytes();
#elif defined(TEENSYDUINO)
  return 0;
#endif
}

long fileSystemLFS::usedBytes() {
#if defined(ESP32)
  return LittleFS.usedBytes();
#elif defined(TEENSYDUINO)
  return 0;
#endif
}