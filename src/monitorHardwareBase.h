#ifndef _MONITOR_BASE_H
#define _MONITOR_BASE_H

#include <Arduino.h>

#include "ewma.h"

class MonitorHardwareBase {
  public:
    void setup(uint32_t voltagePin);
    virtual byte monitorCPUTemp();
    virtual byte monitorMemory();
    virtual byte monitorVoltage();
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
    virtual void setupInternal();
    
    float _cpuTemp = 0.0;
    float _cpuTempMax = 0.0;
    int _memoryHeap = 0;
    int _memoryHeapKb = 0;
    int _memoryHeapInternal = 0;
    int _memoryHeapInternalKb = 0;
    int _memoryHeapMinimum = 0;
    int _memoryHeapMinimumKb = 0;
    int _memoryRam = 0;
    int _memoryRamKb = 0;
    int _memoryStack = 0;
    int _memoryStackKb = 0;
    float _voltage = 0;
    uint8_t _voltagePin = 0;

    ewma _cpuTemp_filter = ewma(0.01); 
    ewma _voltage_filter = ewma(0.01); 
    // MonitorHardwareResourceStruct* _resources;
};

#endif