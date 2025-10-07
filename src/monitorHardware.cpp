#include "monitorHardware.h"

monitorHardware::monitorHardware() {
}

byte monitorHardware::setup(monitorHardwareBase* instance, uint32_t voltagePin) {
    _instance = instance;
    _instance->setup(voltagePin);
    _instance->monitorCPUTemp();
    _instance->monitorMemory();
    _instance->monitorVoltage();

    return 0;
}

byte monitorHardware::monitorCPUTemp() {
    return _instance->monitorCPUTemp();
}

byte monitorHardware::monitorMemory() {
    return _instance->monitorMemory();
}

byte monitorHardware::monitorVoltage() {
    return _instance->monitorVoltage();
}

float monitorHardware::cpuTemp() {
    return _instance->cpuTemp();
}

float monitorHardware::cpuTempMax() {
    return _instance->cpuTempMax();
}

int monitorHardware::memoryHeap() {
    return _instance->memoryHeap();
}

int monitorHardware::memoryHeapKb() {
    return _instance->memoryHeapKb();
}

int monitorHardware::memoryHeapInternal() {
    return _instance->memoryHeapInternal();
}

int monitorHardware::memoryHeapInternalKb() {
    return _instance->memoryHeapInternalKb();
}

int monitorHardware::memoryHeapMinimum() {
    return _instance->memoryHeapMinimum();
}

int monitorHardware::memoryHeapMinimumKb() {
    return _instance->memoryHeapMinimumKb();
}

int monitorHardware::memoryRam() {
    return _instance->memoryRam();
}

int monitorHardware::memoryRamKb() {
    return _instance->memoryRamKb();
}

int monitorHardware::memoryStack() {
    return _instance->memoryStack();
}

int monitorHardware::memoryStackKb() {
    return _instance->memoryStackKb();
}

float monitorHardware::voltage() {
    return _instance->voltage();
}
