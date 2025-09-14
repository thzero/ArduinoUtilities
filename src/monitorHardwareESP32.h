#ifndef _MONITOR_ESP2_H
#define _MONITOR_ESP2_H

#if defined(ESP32)

#include "monitorHardware.h"
#include "monitorHardwareBase.h"

class MonitorHardwareESP32 : public MonitorHardwareBase {
  public:
    MonitorHardwareESP32();
    byte monitorCPUTemp() override;
    byte monitorMemory() override;
    byte monitorVoltage() override;
    
  protected:
    void setupInternal() override;
};

extern MonitorHardware _monitorHardware;

extern void setupMonitorHardware();

#endif

#endif