#ifndef _MONITOR_TEENSY_H
#define _MONITOR_TEENSY_H

#if defined(TEENSYDUINO)

#include <Arduino.h>

#include "ewma.h"
#include "monitorHardwareBase.h"

#define voltage_pinA A9

class MonitorHardwareTeensy : public MonitorHardwareBase {
  public:
    MonitorHardwareTeensy();
    byte monitorCPUTemp();
    byte monitorMemory();
    byte monitorVoltage(int pin);
    
  protected:
    void setupInternal();
    
  private:
    ewma _cpuTemp_filter = ewma(0.01); 
    ewma _voltage_filter = ewma(0.01); 
};

extern MonitorHardwareTeensy _monitorHardware;

extern void setupMonitorHardware(uint8_t pin);

#endif

#endif