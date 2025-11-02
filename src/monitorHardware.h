#ifndef _MONITOR_H
#define _MONITOR_H

#include "monitorHardwareBase.h"

class monitorHardware {
  public:
    monitorHardware();
    int8_t setup(monitorHardwareBase* instance, uint32_t voltagePin);
    int8_t monitorCPUTemp();
    int8_t monitorMemory();
    int8_t monitorVoltage();
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