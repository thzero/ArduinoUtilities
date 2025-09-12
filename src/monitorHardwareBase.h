#ifndef _MONITOR_BASE_H
#define _MONITOR_BASE_H

#include <Arduino.h>

struct MonitorHardwareResourceStruct {
  float cpuTemp = 0.0;
  float cpuTempMax = 0.0;
  int memoryHeap = 0;
  int memoryHeapKb = 0;
  int memoryHeapInternal = 0;
  int memoryHeapInternalKb = 0;
  int memoryHeapMinimum = 0;
  int memoryHeapMinimumKb = 0;
  int memoryRam = 0;
  int memoryRamKb = 0;
  int memoryStack = 0;
  int memoryStackKb = 0;
  float voltage = 0;
};

class MonitorHardwareBase {
  public:
    void setup(MonitorHardwareResourceStruct* resources);
    byte monitorCPUTemp();
    byte monitorMemory();
    byte monitorVoltage(int pin);
    float cpuTemp();
    float cpuTempMax();
    int memoryHeap();
    int memoryHeapKb();
    int memoryHeapInternal();
    int memoryHeapInternalKb();
    int memoryHeapMinimum();
    int memoryHeapMinimumKb();
    int memoryRam();
    int memoryRamKb();
    int memoryStack();
    int memoryStackKb();
    float voltage();
  protected:
    void setupInternal();

    MonitorHardwareResourceStruct* _resources;
    float cpuTempS[10];
    byte cpuTempIndex = 0;
    byte cpuTempIndexMax = 10;
    bool cpuTempComplete = false;
    int voltageS[10];
    byte voltageIndex = 0;
    byte voltageIndexMax = 10;
    bool voltageComplete = false;
};

#endif