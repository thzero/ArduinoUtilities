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
  // float value = 0.0;

  // Converts C to F  --  Error if over 165. It will panic at 170?
  // value  = (tempmonGetTemp() * 9.0f / 5.0f) + 32.0f;

  // // Serial.printf("cpuTempIndex.a: %d\n", cpuTempIndex);
  // cpuTempS[cpuTempIndex] = value; 
  // // Serial.printf("cpuTemp2: %d\n", cpuTempS[cpuTempIndex]);

  // value = 0.0;
  // byte cpuTempIndexMaxT = cpuTempComplete ? cpuTempIndexMax : cpuTempIndex + 1;
  // // Serial.printf("cpuTempIndexMaxT: %d\n", cpuTempIndexMaxT);
  // for (int i = 0; i < cpuTempIndexMaxT; i++) {
  //   value += cpuTempS[i];
  //   // Serial.printf("cpuTempS.%d: %d %d\n", i, cpuTempS[i], value);
  // }
  // // Serial.printf("cpuTemp3: %d\n", value);
  // value = value / cpuTempIndexMax; // average
  // // Serial.printf("cpuTemp4: %d\n", value);

  // cpuTempIndex = cpuTempIndex + 1;
  // if (cpuTempIndex == 10) {
  //   cpuTempComplete = true;
  //   cpuTempIndex = 0;
  // }


  // _resources->cpuTemp = value;
  _resources->cpuTemp = _cpuTemp_filter.filter(tempmonGetTemp());
  // Serial.printf("cpuTemp5: %d\n", _resources->cpuTemp);

  if (_resources->cpuTemp > _resources->cpuTempMax) 
    _resources->cpuTempMax = _resources->cpuTemp;

  // if (_resources->cpuTemp >= 170.0f)
  if (_resources->cpuTemp >= 77.0f) // ~170F
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

  _resources->memoryHeap = heap;
  _resources->memoryRam = psram;
  _resources->memoryStack = stack;
  _resources->memoryHeapKb = heap>>10;
  _resources->memoryRamKb = psram>>10;
  _resources->memoryStackKb = stack>>10;

  return 0;
}

byte MonitorHardwareTeensy::monitorVoltage(int pin) {
  // int value = 0;
  
  // get the samples
  // for (int i = 1; i <= 10; i++)  // ten samples
  //   value = value + analogRead(pin);
  // value = value / 10; // average
  // // Serial.printf("voltage: %d\n", value);

  // // Serial.printf("voltageIndex.a: %d\n", voltageIndex);
  // voltageS[voltageIndex] = value; 
  // // Serial.printf("voltage2: %d\n", voltageS[voltageIndex]);

  // value = 0;
  // byte voltageIndexMaxT = voltageComplete ? voltageIndexMax : voltageIndex + 1;
  // // Serial.printf("voltageIndexMaxT: %d\n", voltageIndexMaxT);
  // for (int i = 0; i < voltageIndexMaxT; i++) {
  //   value += voltageS[i];
  //   // Serial.printf("voltageS.%d: %d %d\n", i, voltageS[i], value);
  // }
  // // Serial.printf("voltage3: %d\n", value);
  // value = value / voltageIndexMax; // average
  // // Serial.printf("voltage4: %d\n", value);

  // voltageIndex = voltageIndex + 1;
  // if (voltageIndex == voltageIndexMax) {
  //   voltageComplete = true;
  //   voltageIndex = 0;
  // }
  // // Serial.println();

  _resources->voltage = _voltage_filter.filter(analogRead(pin));

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
  // _resources->voltage = vin;

  // Serial.printf("voltage: %f\n", _resources->voltage);
  if (_resources->voltage <= 10.0f) {
    // Serial.printf("voltage %f is below 10.0%.\n", _resources->voltage);
    return 1;
  }
  // Serial.println("voltage is fine.");
  return 0;
}

void MonitorHardwareTeensy::setupInternal() {
  Serial.println(F("Setup monitor..."));
  Serial.println(F("...monitor finished."));
}

MonitorHardwareTeensy _monitorHardware;
MonitorHardwareResourceStruct _monitorHardwareResources;

void setupMonitorHardware(uint8_t pin) {
   // This is accepting input from outside
   // file:///D:/users/thzero/Downloads/Schematic_RocketTalk%205.1%20Schematic%20PDF.pdf
   // Its taking ground output through a 100k resistor
   // Its taking V output from battery through a 300k resistor
   // Sending that through the L4941BDT-TR to the VIN through two capcitors 10uF and 4F
  pinMode(pin, INPUT);
  _monitorHardware.setup(&_monitorHardwareResources);
  _monitorHardware.monitorCPUTemp();
  _monitorHardware.monitorMemory();
  _monitorHardware.monitorVoltage(0);
}

#endif