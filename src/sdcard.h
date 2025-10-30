#ifndef _SDCARD_H
#define _SDCARD_H

#if defined(TEENSYDUINO)

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SdFat.h>
#include "sdios.h"

class sdcard {
  public:
    sdcard();
    byte setup();
    byte cardInfo();
    bool deleteFile(const char* filePath);
    FsFile openRead(const char* filePath);
    FsFile openWrite(const char* filePath);
    bool readJson(const char* filePath, JsonDocument& doc);
    bool writeJson(const char* filePath, JsonDocument& doc);
  private:
    void clearSerialInput();
    void errorPrint();
    bool printCid();
    bool printCsd();
    bool printMbr();
    void printCardType();
    void printVolume();

    SdFs _sd;
    
    cid_t _cid;
    csd_t _csd;
    uint32_t _ocr;
    uint32_t _eraseSize;
};

#endif

#endif