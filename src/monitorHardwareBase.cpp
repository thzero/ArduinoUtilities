#include "monitorHardwareBase.h"

void MonitorHardwareBase::setup(uint32_t voltagePin) {
    _voltagePin = voltagePin;
    setupInternal();
}

void MonitorHardwareBase::setupInternal() {
  Serial.println(F("Setup monitor..."));
  Serial.println(F("...monitor finished."));
}

byte MonitorHardwareBase::monitorCPUTemp() {
  return 0;
}

byte MonitorHardwareBase::monitorMemory() {
  return 0;
}

byte MonitorHardwareBase::monitorVoltage() {
  return 0;
}

float MonitorHardwareBase::cpuTemp() {
    // return _resources->cpuTemp;
    return _cpuTemp;
}

float MonitorHardwareBase::cpuTempMax() {
    // return _resources->cpuTempMax;
    return _cpuTempMax;
}

int MonitorHardwareBase::memoryHeap() {
    // return _resources->memoryHeap;
    return _memoryHeap;
}

int MonitorHardwareBase::memoryHeapKb() {
    // return _resources->memoryHeap >> 10;
    return _memoryHeap >> 10;
}

int MonitorHardwareBase::memoryHeapInternal() {
    // return _resources->memoryHeapInternal;
    return _memoryHeapInternal;
}

int MonitorHardwareBase::memoryHeapInternalKb() {
    // return _resources->memoryHeapInternal >> 10;
    return _memoryHeapInternal >> 10;
}

int MonitorHardwareBase::memoryHeapMinimum() {
    // return _resources->memoryHeapMinimum;
    return _memoryHeapMinimum;
}

int MonitorHardwareBase::memoryHeapMinimumKb() {
    // return _resources->memoryHeapMinimum >> 10;
    return _memoryHeapMinimum >> 10;
}

int MonitorHardwareBase::memoryRam() {
    // return _resources->memoryRam;
    return _memoryRam;
}

int MonitorHardwareBase::memoryRamKb() {
    // return _resources->memoryRam >> 10;
    return _memoryRam >> 10;
}

int MonitorHardwareBase::memoryStack() {
    // return _resources->memoryStack;
    return _memoryStack;
}

int MonitorHardwareBase::memoryStackKb() {
    // return _resources->memoryStack >> 10;
    return _memoryStack >> 10;
}

float MonitorHardwareBase::voltage() {
    // return _resources->voltage;
    return _voltage;
}
