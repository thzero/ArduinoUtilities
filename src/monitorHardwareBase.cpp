#include "monitorHardwareBase.h"

void monitorHardwareBase::setup(uint32_t voltagePin) {
    _voltagePin = voltagePin;
    setupInternal();
}

void monitorHardwareBase::setupInternal() {
  Serial.println(F("Setup monitor..."));
  Serial.println(F("...monitor finished."));
}

byte monitorHardwareBase::monitorCPUTemp() {
  return 0;
}

byte monitorHardwareBase::monitorMemory() {
  return 0;
}

byte monitorHardwareBase::monitorVoltage() {
  return 0;
}

float monitorHardwareBase::cpuTemp() {
    return _cpuTemp;
}

float monitorHardwareBase::cpuTempMax() {
    return _cpuTempMax;
}

int monitorHardwareBase::memoryHeap() {
    return _memoryHeap;
}

int monitorHardwareBase::memoryHeapKb() {
    return _memoryHeap >> 10;
}

int monitorHardwareBase::memoryHeapInternal() {
    return _memoryHeapInternal;
}

int monitorHardwareBase::memoryHeapInternalKb() {
    return _memoryHeapInternal >> 10;
}

int monitorHardwareBase::memoryHeapMinimum() {
    return _memoryHeapMinimum;
}

int monitorHardwareBase::memoryHeapMinimumKb() {
    return _memoryHeapMinimum >> 10;
}

int monitorHardwareBase::memoryRam() {
    return _memoryRam;
}

int monitorHardwareBase::memoryRamKb() {
    return _memoryRam >> 10;
}

int monitorHardwareBase::memoryStack() {
    return _memoryStack;
}

int monitorHardwareBase::memoryStackKb() {
    return _memoryStack >> 10;
}

float monitorHardwareBase::voltage() {
    return _voltage;
}
