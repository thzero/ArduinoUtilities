#include "monitorHardwareBase.h"

void MonitorHardwareBase::setup(MonitorHardwareResourceStruct* resources) {
    _resources = resources;
    setupInternal();
}

void MonitorHardwareBase::setupInternal() {
}

float MonitorHardwareBase::cpuTemp() {
    return _resources->cpuTemp;
}

float MonitorHardwareBase::cpuTempMax() {
    return _resources->cpuTempMax;
}

int MonitorHardwareBase::memoryHeap() {
    return _resources->memoryHeap;
}

int MonitorHardwareBase::memoryHeapKb() {
    return _resources->memoryHeap >> 10;
}

int MonitorHardwareBase::memoryHeapInternal() {
    return _resources->memoryHeapInternal;
}

int MonitorHardwareBase::memoryHeapInternalKb() {
    return _resources->memoryHeapInternal >> 10;
}

int MonitorHardwareBase::memoryHeapMinimum() {
    return _resources->memoryHeapMinimum;
}

int MonitorHardwareBase::memoryHeapMinimumKb() {
    return _resources->memoryHeapMinimum >> 10;
}

int MonitorHardwareBase::memoryRam() {
    return _resources->memoryRam;
}

int MonitorHardwareBase::memoryRamKb() {
    return _resources->memoryRam >> 10;
}

int MonitorHardwareBase::memoryStack() {
    return _resources->memoryStack;
}

int MonitorHardwareBase::memoryStackKb() {
    return _resources->memoryStack >> 10;
}

float MonitorHardwareBase::voltage() {
    return _resources->voltage;
}
