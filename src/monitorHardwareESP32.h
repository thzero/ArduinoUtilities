#ifndef _MONITOR_ESP2_H
#define _MONITOR_ESP2_H

#if defined(ESP32)

#include "monitorHardware.h"
#include "monitorHardwareBase.h"

class monitorHardwareESP32 : public monitorHardwareBase {
  public:
    monitorHardwareESP32();
    byte monitorCPUTemp() override;
    byte monitorMemory() override;
    byte monitorVoltage() override;
    
  protected:
    void setupInternal() override;
};

extern monitorHardware _monitorHardware;

extern void setupMonitorHardware();

#endif

#endif