#ifndef _MONITOR_H
#define _MONITOR_H

#include <Arduino.h>

#include "monitorHardwareBase.h"

class MonitorHardware {
  public:
    MonitorHardware();
    void setup(MonitorHardwareBase* instance, uint32_t voltagePin);
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
    MonitorHardwareBase* _instance;
};

#endif