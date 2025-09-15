#ifndef _MONITOR_H
#define _MONITOR_H

#include <Arduino.h>

#include "monitorHardwareBase.h"

class monitorHardware {
  public:
    monitorHardware();
    byte setup(monitorHardwareBase* instance, uint32_t voltagePin);
    byte monitorCPUTemp();
    byte monitorMemory();
    byte monitorVoltage();
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
    monitorHardwareBase* _instance;
};

#endif