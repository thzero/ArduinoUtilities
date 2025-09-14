#if defined(TEENSYDUINO)

#include <string.h>

#include <utilities.h>
#include "monitorHardwareTeensy.h"
#include "utilitiesTeensy.h"

extern float tempmonGetTemp(void); 

// extern unsigned long _heap_end;
// extern char *__brkval;

// int freeram() {
//   return (char *)&_heap_end - __brkval;
// }

extern char _itcm_block_count [];
extern uint32_t external_psram_size;

MonitorHardwareTeensy::MonitorHardwareTeensy() {
}

byte MonitorHardwareTeensy::monitorCPUTemp() {
  // Serial.printf("monitorCPUTemp3: %f\n", _cpuTemp);
  _cpuTemp = _cpuTemp_filter.filter(tempmonGetTemp());
  // Serial.printf("cpuTemp5: %d\n", _cpuTemp);

  if (_cpuTemp > _cpuTempMax) 
    _cpuTempMax = _cpuTemp;

  // if (_cpuTemp >= 170.0f)
  if (_cpuTemp >= 77.0f) // ~170F
    return 1; 
  return 0;
}

byte MonitorHardwareTeensy::monitorMemory() {
  // constexpr auto RAM_BASE   = 0x2020'0000;
  // constexpr auto RAM_SIZE   = 512 << 10;
  // constexpr auto FLASH_BASE = 0x6000'0000;
  // constexpr auto FLASH_SIZE = 8 << 20;

  // extern char _stext[], _etext[], _sbss[], _ebss[], _sdata[], _edata[], _estack[], _heap_start[], _heap_end[], _extram_start[], _extram_end[], *__brkval;
  extern char _ebss[], _heap_end[], _extram_start[], _extram_end[], *__brkval;

  char* sp = (char*)__builtin_frame_address(0);

  // Serial.printf("_stext        %08x\n",      _stext);
  // Serial.printf("_etext        %08x +%db\n", _etext, _etext-_stext);
  // Serial.printf("_sdata        %08x\n",      _sdata);
  // Serial.printf("_edata        %08x +%db\n", _edata, _edata-_sdata);
  // Serial.printf("_sbss         %08x\n",      _sbss);
  // Serial.printf("_ebss         %08x +%db\n", _ebss, _ebss-_sbss);
  // Serial.printf("curr stack    %08x +%db\n", sp, sp-_ebss);
  // Serial.printf("_estack       %08x +%db\n", _estack, _estack-sp);
  // Serial.printf("_heap_start   %08x\n",      _heap_start);
  // Serial.printf("__brkval      %08x +%db\n", __brkval, __brkval-_heap_start);
  // Serial.printf("_heap_end     %08x +%db\n", _heap_end, _heap_end-__brkval);
  // Serial.printf("_extram_start %08x\n",      _extram_start);
  // Serial.printf("_extram_end   %08x +%db\n", _extram_end,
  //                                             _extram_end-_extram_start);
  // Serial.printf("\n");

  // Serial.printf("<ITCM>  %08x .. %08x\n",
  //         _stext, _stext + ((int) _itcm_block_count << 15) - 1);
  // Serial.printf("<DTCM>  %08x .. %08x\n",
  //         _sdata, _estack - 1);
  // Serial.printf("<RAM>   %08x .. %08x\n",
  //         RAM_BASE, RAM_BASE + RAM_SIZE - 1);
  // Serial.printf("<FLASH> %08x .. %08x\n",
  //         FLASH_BASE, FLASH_BASE + FLASH_SIZE - 1);
  // Serial.printf("<PSRAM> %08x .. %08x\n",
  //         _extram_start, _extram_start + (external_psram_size<<20) - 1);
  // Serial.printf("\n");

  auto stack = sp -_ebss,
        heap = _heap_end -__brkval,
        psram = _extram_start + (external_psram_size<<20) - _extram_end;
  // Serial.printf("avail STACK %8d b %5d kb\n", stack, stack>>10);
  // Serial.printf("avail HEAP  %8d b %5d kb\n", heap, heap>>10);
  // Serial.printf("avail PSRAM %8d b %5d kb\n", psram, psram>>10);

  _memoryHeap = heap;
  _memoryRam = psram;
  _memoryStack = stack;
  _memoryHeapKb = heap>>10;
  _memoryRamKb = psram>>10;
  _memoryStackKb = stack>>10;

  return 0;
}

byte MonitorHardwareTeensy::monitorVoltage() {
  _voltage = _voltage_filter.filter(analogRead(_voltagePin));

  // float vin = 0.0;
  // if (value < 490) 
  //   vin = 0.0f;
  // else if (value > 580) 
  //   vin = 100.0f;
  // else if (value <= 580 && value >= 490) {
  //   float tempf = (float)value - (float)490.0;
  //   // Serial.printf("voltage.temp: %f\n", tempf);
  //   vin = (float) tempf * (float) 1.1111;  // 90 is spread. 100/90 = 1.1111
  //   // Serial.printf("voltage.vin.1: %f\n", vin);
  //   if (vin > 100) 
  //     vin = 100;
  //   if (vin < 0) 
  //     vin = 0;
  //   // Serial.printf("voltage.vin.2: %f\n", vin);
  // }
  // // Serial.printf("voltage5: %f\n", vin);
  // _voltage = vin;

  // Serial.printf("voltage: %f\n", _voltage);
  if (_voltage <= 10.0f) {
    // Serial.printf("voltage %f is below 10.0%.\n", _voltage);
    return 1;
  }
  // Serial.println("voltage is fine.");
  return 0;
}

void MonitorHardwareTeensy::setupInternal() {
}

// MonitorHardware<MonitorHardwareTeensy> _monitorHardware;

// void setupMonitorHardware(uint8_t pin) {
//    // This is accepting input from outside
//    // file:///D:/users/thzero/Downloads/Schematic_RocketTalk%205.1%20Schematic%20PDF.pdf
//    // Its taking ground output through a 100k resistor
//    // Its taking V output from battery through a 300k resistor
//    // Sending that through the L4941BDT-TR to the VIN through two capcitors 10uF and 4F
//   pinMode(2, INPUT);
//   _monitorHardware.setup();
// }

MonitorHardwareTeensy _monitorHardwareTeensy;
MonitorHardware _monitorHardware;
void setupMonitorHardware(uint8_t voltagePin) {
   // This is accepting input from outside
   // file:///D:/users/thzero/Downloads/Schematic_RocketTalk%205.1%20Schematic%20PDF.pdf
   // Its taking ground output through a 100k resistor
   // Its taking V output from battery through a 300k resistor
   // Sending that through the L4941BDT-TR to the VIN through two capcitors 10uF and 4F
  pinMode(voltagePin, INPUT);
  _monitorHardware.setup(&_monitorHardwareTeensy, voltagePin);
}

#endif