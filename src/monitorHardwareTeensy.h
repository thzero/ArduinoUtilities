#ifndef _MONITOR_TEENSY_H
#define _MONITOR_TEENSY_H

#if defined(TEENSYDUINO)

#include <Arduino.h>

#include "monitorHardware.h"
#include "monitorHardwareBase.h"

#define voltage_pinA A9

class monitorHardwareTeensy : public monitorHardwareBase {
  public:
    monitorHardwareTeensy();
    byte monitorCPUTemp() override;
    byte monitorMemory() override;
    byte monitorVoltage() override;
    
  protected:
    void setupInternal() override; 
};

extern monitorHardware _monitorHardware;

extern void setupMonitorHardware(uint8_t pin);

#endif

#endif