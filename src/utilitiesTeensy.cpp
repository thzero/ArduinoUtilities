#if defined(TEENSYDUINO)

#include <string.h>

#include <utilities.h>
#include "utilitiesTeensy.h"

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

// https://github.com/sstaub/TeensyID
#if defined ARDUINO_TEENSY40 || defined ARDUINO_TEENSY41
void fetchMAC(uint8_t *mac) { // there are 2 MAC addresses each 48bit 
  uint32_t m1 = HW_OCOTP_MAC1;
  uint32_t m2 = HW_OCOTP_MAC0;
  mac[0] = m1 >> 8;
  mac[1] = m1 >> 0;
  mac[2] = m2 >> 24;
  mac[3] = m2 >> 16;
  mac[4] = m2 >> 8;
  mac[5] = m2 >> 0;
}
#else
void fetchMAC(uint8_t *mac) {
  uint8_t serial[4];
  teensySN(serial);
  mac[0] = 0x04;
  mac[1] = 0xE9;
  mac[2] = 0xE5;
  mac[3] = serial[1];
  mac[4] = serial[2];
  mac[5] = serial[3];
}
#endif

#endif