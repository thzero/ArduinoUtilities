#include "monitorHardware.h"

MonitorHardware::MonitorHardware() {
}

void MonitorHardware::setup(MonitorHardwareBase* instance, uint32_t voltagePin) {
    _instance = instance;
    _instance->setup(voltagePin);
    _instance->monitorCPUTemp();
    _instance->monitorMemory();
    _instance->monitorVoltage();
}

byte MonitorHardware::monitorCPUTemp() {
    return _instance->monitorCPUTemp();
}

byte MonitorHardware::monitorMemory() {
    return _instance->monitorMemory();
}

byte MonitorHardware::monitorVoltage() {
    return _instance->monitorVoltage();
}

float MonitorHardware::cpuTemp() {
    return _instance->cpuTemp();
}

float MonitorHardware::cpuTempMax() {
    return _instance->cpuTempMax();
}

int MonitorHardware::memoryHeap() {
    return _instance->memoryHeap();
}

int MonitorHardware::memoryHeapKb() {
    return _instance->memoryHeap();
}

int MonitorHardware::memoryHeapInternal() {
    return _instance->memoryHeapInternal();
}

int MonitorHardware::memoryHeapInternalKb() {
    return _instance->memoryHeapInternal();
}

int MonitorHardware::memoryHeapMinimum() {
    return _instance->memoryHeapMinimum();
}

int MonitorHardware::memoryHeapMinimumKb() {
    return _instance->memoryHeapMinimum();
}

int MonitorHardware::memoryRam() {
    return _instance->memoryRam();
}

int MonitorHardware::memoryRamKb() {
    return _instance->memoryRam();
}

int MonitorHardware::memoryStack() {
    return _instance->memoryStack();
}

int MonitorHardware::memoryStackKb() {
    return _instance->memoryStack();
}

float MonitorHardware::voltage() {
    return _instance->voltage();
}
