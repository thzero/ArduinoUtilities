#if defined(ESP32)

#include <Arduino.h>

#include <debug.h>
#include "monitorHardwareESP32.h"

MonitorHardwareESP32::MonitorHardwareESP32() {
}

byte MonitorHardwareESP32::monitorCPUTemp() {
  return 0;
}

byte MonitorHardwareESP32::monitorMemory() {
  _resources->memoryHeap = esp_get_free_heap_size();
  _resources->memoryHeapInternal = esp_get_free_internal_heap_size();
  _resources->memoryHeapMinimum = esp_get_minimum_free_heap_size();

  return 0;
}

byte MonitorHardwareESP32::monitorVoltage(int pin) {
  // Voltage in volts not mv, using a 1:1 divider so 500 division.
  _voltage = _voltage_filter.filter(analogReadMilliVolts(10) / 500);

  // memmove(&_voltageRolling[0], &_voltageRolling[1], (_voltageCapacity -1) * sizeof(_voltageRolling[0]));
  // _voltageRolling[_voltageCapacity - 1] = analogReadMilliVolts(10) / 500;

  _resources->voltage = _voltage;

  // Serial.printf("voltage: %f\n", _resources->voltage);
  if (_resources->voltage <= 2.0f) {
    // Serial.printf("voltage %f is below 2.0%.\n", _resources->voltage);
    return 1;
  }

  // Serial.println("voltage is fine.");
  return 0;
}

void MonitorHardwareESP32::setupInternal() {
  Serial.println(F("Setup monitor..."));

  Serial.println(F("\t...voltage..."));
  /*
   .. Setup 1:1 ratio voltage divider from VBAT -> Pin 10 -> Pin 6
  */
  // It turns out that pins using ADC2 instead of ADC1 can conflict with the WiFi so it's good practice to use a ACD1 port.
  // Also if you use the analogReadMilliVolts(PIN) option instead of working it out yourself it is calibrated for the fact the pin read is not fully linear and gives a nice exact voltage.

  // Put voltage divider down the VBAT side which is easier from VBAT to pin10, and then the second resistor from pin10 to pin 6.
  // Just set pin6 to a LOW output and pin10 to an input.  Normally go for more than 33k's
  analogReadResolution(12);
  pinMode(10, INPUT);   
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  
  _voltage = _voltage_filter.filter(analogReadMilliVolts(10) / 500);
  Serial.println(F("\t...voltage finished"));

  Serial.println(F("...monitor finished."));
}

MonitorHardwareESP32 _monitorHardware;
MonitorHardwareResourceStruct _monitorHardwareResources;

void setupMonitorHardware() {
  _monitorHardware.setup(&_monitorHardwareResources);
  _monitorHardware.monitorCPUTemp();
  _monitorHardware.monitorMemory();
  _monitorHardware.monitorVoltage(0);
}

#endif