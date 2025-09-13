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
  File file = LittleFS.open(path);
  if (!file || file.isDirectory()) {
    Serial.printf(F("...failed to open the %s.\n"), path);
    return File();
  }

  return file;
}

bool fileSystemLFS::setup() {
  Serial.println(F("\tLittleFS begin..."));
  if (!LittleFS.begin(false)) {
    Serial.println(F("\tLittleFS mount failed"));
    Serial.println(F("\tDid not find filesystem; starting format"));

    // format if begin fails
    Serial.println(F("\tLittleFS trying to format..."));
    if (!LittleFS.begin(true)) {
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
  return LittleFS.totalBytes();
}

long fileSystemLFS::usedBytes() {
  return LittleFS.usedBytes();
}