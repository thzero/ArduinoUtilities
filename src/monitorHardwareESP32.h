#ifndef _MONITOR_H
#define _MONITOR_H

#if defined(ESP32)

#include "ewma.h"
#include "monitorHardwareBase.h"

class MonitorHardwareESP32 : public MonitorHardwareBase {
  public:
    MonitorHardwareESP32();
    byte monitorCPUTemp();
    byte monitorMemory();
    byte monitorVoltage(int pin);
    
  protected:
    void setupInternal();
    
  private:
    uint32_t _heap;
    uint32_t _heap_internal;
    uint32_t _heap_minimum;
    uint32_t _voltage;
    uint32_t _voltageRolling[201];
    const size_t _voltageCapacity = 200;

    ewma _voltage_filter = ewma(0.01); 
};

extern MonitorHardwareESP32 _monitorHardware;

extern void setupMonitorHardware();

#endif

#endif