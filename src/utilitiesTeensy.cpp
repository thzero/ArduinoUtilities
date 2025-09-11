#if defined(TEENSYDUINO)

#include <string.h>

#include <utilities.h>
#include "utilitiesTeensy.h"

extern float tempmonGetTemp(void); 

// extern unsigned long _heap_end;
// extern char *__brkval;

// int freeram() {
//   return (char *)&_heap_end - __brkval;
// }

extern char _itcm_block_count [];
extern uint32_t external_psram_size;

void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
      temp = str[i];
      str[i] = str[j];
      str[j] = temp;
      i++;
      j--;
    }
}

void bToStr(byte x, char res[20], int d)
{
    if (x == 0) {
      strcpy(res,"0");
      return;
    }
  
    int i = 0;
    while (x) {
      res[i++] = (x % 10) + '0';
      x = x / 10;
    }

    while (i < d)
      res[i++] = '0';

    reverse(res, i);
    res[i] = '\0';
}

byte crashReport() {
  if (CrashReport) {
    Serial.print(CrashReport);
#ifdef DEBUG
    while(1); // hard stop if debug mode
#endif 
    return 1;
  } 
  
  return 0; //healthy
}

void iToStr(int x, char res[20], int d)
{
    if (x == 0) {
      strcpy(res, "0");
      if (d == 2) 
        strcpy(res, "00");
      if (d == 3) 
        strcpy(res, "000");
      return;
    }

    bool neg = false;
    if (x < 0) {
      x = x * -1;
      neg = true;
    }
    
    int i = 0;
    while (x) {
      res[i++] = (x % 10) + '0';
      x = x / 10;
    }
    while (i < d)
        res[i++] = '0';

    reverse(res, i);
    res[i] = '\0';

    if (neg) {
      char temp1[20];
      strcpy(temp1, (const char*)F("-"));
      strcat(temp1, res);
      strcpy(res, temp1);
    }
}
 
int intToStr(int x, char str[], int d) //only used for the float function
{
    int i = 0;
    while (x) {
      str[i++] = (x % 10) + '0';
      x = x / 10;
    }
    while (i < d)
      str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

void fToStr(float n, char res[20], int afterpoint)
{
    bool neg = false;
    if (n < 0) {
      n = n * -1;
      neg = true;
    }

    int ipart = (int)n;
    int i = 0;
    float fpart = n - (float)ipart;
    if (ipart == 0) {
      strcpy(res,"0");
      i = 1;
    } 
    else
      i = intToStr(ipart, res, 0);

    if (afterpoint != 0) {
        res[i] = '.';
        fpart = fpart * pow(10, afterpoint);
        intToStr((int)fpart, res + i + 1, afterpoint);
    }

    if (neg) {
      char temp1[20];
      strcpy(temp1, (const char*)F("-"));
      strcat(temp1, res);
      strcpy(res, temp1);
    }
}

void ulongToStr(unsigned long x, char res[20], int d)
{
    if (x == 0) {
      strcpy(res, (const char*)F("0"));
      if (d == 2) 
        strcpy(res, (const char*)F("00"));
      if (d == 3) 
        strcpy(res, (const char*)F("000"));
      return;
    }

    int i = 0;
    while (x) {
      res[i++] = (x % 10) + '0';
      x = x / 10;
    }
    while (i < d)
      res[i++] = '0';

    reverse(res, i);
    res[i] = '\0';
}

void dumpBuffers(const char *title, const uint8_t *buf, size_t start, size_t len)
{
    // show title and range
    if (len == 1)
        printf("%s [%d]: ", title, start);
    else
        printf("%s [%d-%d]: ", title, start, start + len - 1);

    // show data in the range
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", buf[start + i]);
    }
    printf("\n");
}

bool verifyAndDumpDifference(const char *a_title, const uint8_t *a_buf, size_t a_size, const char *b_title, const uint8_t *b_buf, size_t b_size)
{
    bool verified = true;

    if (a_size != b_size) {
        printf("received data size does not match: expected = %d / actual = %d\n", a_size, b_size);
        return false;
    }

    for (size_t i = 0; i < a_size; i++) {
        // if a_buf and b_buf is same, continue
        if (a_buf[i] == b_buf[i]) {
            continue;
        }

        verified = false;

        // if a_buf[i] and b_buf[i] is not same, check the range that has difference
        size_t j = 1;
        for (; i + j < a_size; ++j) {
            if (a_buf[i + j] != b_buf[i + j]) {
                break;
            }
        }

        // dump different data range
        dumpBuffers(a_title, a_buf, i, j);
        dumpBuffers(b_title, b_buf, i, j);

        // restart from next same index (-1 considers i++ in for())
        i += j - 1;
    }
    return verified;
}

void initializeBuffers(uint8_t *tx, uint8_t *rx, size_t size, size_t offset)
{
    if (tx) {
        for (size_t i = 0; i < size; i++) {
            tx[i] = (i + offset) & 0xFF;
        }
    }
    if (rx) {
        memset(rx, 0, size);
    }
}

monitorTeensy::monitorTeensy() {
}

byte monitorTeensy::monitorCPUTemp(monitorWorkingStruct *working) {
  float value = 0.0;

  // Converts C to F  --  Error if over 165. It will panic at 170?
  value  = (tempmonGetTemp() * 9.0f / 5.0f) + 32.0f;

  // Serial.printf("cpuTempIndex.a: %d\n", cpuTempIndex);
  cpuTempS[cpuTempIndex] = value; 
  // Serial.printf("cpuTemp2: %d\n", cpuTempS[cpuTempIndex]);

  value = 0.0;
  byte cpuTempIndexMaxT = cpuTempComplete ? cpuTempIndexMax : cpuTempIndex + 1;
  // Serial.printf("cpuTempIndexMaxT: %d\n", cpuTempIndexMaxT);
  for (int i = 0; i < cpuTempIndexMaxT; i++) {
    value += cpuTempS[i];
    // Serial.printf("cpuTempS.%d: %d %d\n", i, cpuTempS[i], value);
  }
  // Serial.printf("cpuTemp3: %d\n", value);
  value = value / cpuTempIndexMax; // average
  // Serial.printf("cpuTemp4: %d\n", value);

  cpuTempIndex = cpuTempIndex + 1;
  if (cpuTempIndex == 10) {
    cpuTempComplete = true;
    cpuTempIndex = 0;
  }

  working->cpuTemp = value;
  // Serial.printf("cpuTemp5: %d\n", working->cpuTemp);

  if (working->cpuTemp > working->cpuTempMax) 
    working->cpuTempMax = working->cpuTemp;

  if (working->cpuTemp >= 170.0f)
    return 1; 
  return 0;
}

byte monitorTeensy::monitorMemory(monitorWorkingStruct *working) {
  // constexpr auto RAM_BASE   = 0x2020'0000;
  // constexpr auto RAM_SIZE   = 512 << 10;
  // constexpr auto FLASH_BASE = 0x6000'0000;
  // constexpr auto FLASH_SIZE = 8 << 20;

  // extern char _stext[], _etext[], _sbss[], _ebss[], _sdata[], _edata[], _estack[], _heap_start[], _heap_end[], _extram_start[], _extram_end[], *__brkval;
  extern char _ebss[], _heap_end[], _extram_start[], _extram_end[], *__brkval;

  // char* sp = __builtin_frame_address(0);
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

  working->memoryHeap = heap;
  working->memoryRam = psram;
  working->memoryStack = stack;
  working->memoryHeapKb = heap>>10;
  working->memoryRamKb = psram>>10;
  working->memoryStackKb = stack>>10;

  return 0;
}

byte monitorTeensy::monitorVoltage(monitorWorkingStruct *working, int pin) {
  int value = 0;
  
  // get the samples
  for (int i = 1; i <= 10; i++)  // ten samples
    value = value + analogRead(pin);
  value = value / 10; // average
  // Serial.printf("voltage: %d\n", value);

  // Serial.printf("voltageIndex.a: %d\n", voltageIndex);
  voltageS[voltageIndex] = value; 
  // Serial.printf("voltage2: %d\n", voltageS[voltageIndex]);

  value = 0;
  byte voltageIndexMaxT = voltageComplete ? voltageIndexMax : voltageIndex + 1;
  // Serial.printf("voltageIndexMaxT: %d\n", voltageIndexMaxT);
  for (int i = 0; i < voltageIndexMaxT; i++) {
    value += voltageS[i];
    // Serial.printf("voltageS.%d: %d %d\n", i, voltageS[i], value);
  }
  // Serial.printf("voltage3: %d\n", value);
  value = value / voltageIndexMax; // average
  // Serial.printf("voltage4: %d\n", value);

  voltageIndex = voltageIndex + 1;
  if (voltageIndex == voltageIndexMax) {
    voltageComplete = true;
    voltageIndex = 0;
  }
  // Serial.println();

  float vin = 0.0;
  if (value < 490) 
    vin = 0.0f;
  else if (value > 580) 
    vin = 100.0f;
  else if (value <= 580 && value >= 490) {
    float tempf = (float)value - (float)490.0;
    // Serial.printf("voltage.temp: %f\n", tempf);
    vin = (float) tempf * (float) 1.1111;  // 90 is spread. 100/90 = 1.1111
    // Serial.printf("voltage.vin.1: %f\n", vin);
    if (vin > 100) 
      vin = 100;
    if (vin < 0) 
      vin = 0;
    // Serial.printf("voltage.vin.2: %f\n", vin);
  }
  // Serial.printf("voltage5: %f\n", vin);
  working->voltage = vin;

  // Serial.printf("voltage: %f\n", working->voltage);
  if (working->voltage <= 10.0f) {
    // Serial.printf("voltage %f is below 10.0%.\n", working->voltage);
    return 1;
  }
  // Serial.println("voltage is fine.");
  return 0;
}

monitorTeensy _monitorTeensy;
#endif