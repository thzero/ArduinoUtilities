#include <Arduino.h>
#include <limits.h>  //for LONG_MIN

#include "utilities.h"

unsigned int convertByteArrayToUnsignedInt(uint8_t *byteArray) {
  unsigned int value = 0;
  for (size_t i = 0; i < sizeof(unsigned int); i++) {
    // Serial.printf("%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
    value |= byteArray[i] << (i * 8); // Combine bytes
  }
  // Serial.println();
  return value;
}

void convertPrintByteArrayUnsignedInt(uint8_t *byteArray) {
  for (size_t i = 0; i < sizeof(unsigned int); i++) {
    Serial.printf("%d ", byteArray[i]);
    Serial.printf("%d \n", byteArray[i]);
  }
  Serial.println();
}

unsigned long convertByteArrayToUnsignedLong(uint8_t *byteArray) {
  unsigned long value = 0;
  for (size_t i = 0; i < sizeof(unsigned long); i++) {
    // Serial.printf("%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
    value |= byteArray[i] << (i * 8); // Combine bytes
  }
  // Serial.println();
  return value;
}

void convertPrintUnsignedByteArrayUnsignedLong(uint8_t *byteArray) {
  for (size_t i = 0; i < sizeof(unsigned long); i++) {
    Serial.printf("%d \n", byteArray[i]);
  }
  Serial.println();
}

int convertUnsignedByteArrayToInt(uint8_t *byteArray) {
  int value = 0;
  for (size_t i = 0; i < sizeof(int); i++) {
    // Serial.printf("%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
    value |= byteArray[i] << (i * 8); // Combine bytes
  }
  // Serial.println();
  return value;
}

uint16_t convertUnsignedByteArrayToShort(uint8_t *byteArray) {
  uint16_t value = 0;
  for (size_t i = 0; i < sizeof(uint16_t); i++) {
    // Serial.printf("%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
    value |= byteArray[i] << (i * 8); // Combine bytes
  }
  // Serial.println();
  return value;
}

unsigned int convertUnsignedByteArrayToUnsignedInt(uint8_t *byteArray) {
  unsigned int value = 0;
  for (size_t i = 0; i < sizeof(unsigned int); i++) {
    // Serial.printf("%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
    value |= byteArray[i] << (i * 8); // Combine bytes
  }
  // Serial.println();
  return value;
}

unsigned long convertUnsignedByteArrayToUnsignedLong(uint8_t *byteArray) {
  unsigned long value = 0;
  for (size_t i = 0; i < sizeof(unsigned long); i++) {
    // Serial.printf("%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
    value |= byteArray[i] << (i * 8); // Combine bytes
  }
  // Serial.println();
  return value;
}

uint16_t convertUnsignedByteArrayToUnsignedShort(uint8_t *byteArray) {
  unsigned long value = 0;
  for (size_t i = 0; i < sizeof(uint16_t); i++) {
    // Serial.printf("%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
    value |= byteArray[i] << (i * 8); // Combine bytes
  }
  // Serial.println();
  return value;
}

void convertUnsignedIntToByteArray(unsigned int value, uint8_t *byteArray) {
  for (size_t i = 0; i < sizeof(unsigned int); i++) {
    // Serial.printf("%d ", ((value >> (i * 8)) & 0xFF));
    byteArray[i] = (value >> (i * 8)) & 0xFF; // Extract each byte
    // Serial.printf("a%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
  }
  // Serial.println();
}

void convertUnsignedLongToByteArray(unsigned long value, uint8_t *byteArray) {
  // Serial.println(F("convertUnsignedLongToByteArray"));
  // Serial.printf("%d \n", sizeof(unsigned long));
  // Serial.printf("%d \n", sizeof(byteArray));
  for (size_t i = 0; i < sizeof(unsigned long); i++) {
    // Serial.printf("%d ", ((value >> (i * 8)) & 0xFF));
    byteArray[i] = (value >> (i * 8)) & 0xFF; // Extract each byte
    // Serial.printf("a%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
  }
  // Serial.println();
}

void convertUnsignedIntToUnsignedByteArray(unsigned int value, uint8_t *byteArray) {
  for (size_t i = 0; i < sizeof(unsigned int); i++) {
    // Serial.printf("%d ", ((value >> (i * 8)) & 0xFF));
    byteArray[i] = (value >> (i * 8)) & 0xFF; // Extract each byte
    // Serial.printf("a%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
  }
  // Serial.println();
}

void convertUnsignedLongToUnsignedByteArray(unsigned long value, uint8_t *byteArray) {
  // Serial.println(F("convertUnsignedLongToByteArray"));
  // Serial.printf("%d \n", sizeof(unsigned long));
  // Serial.printf("%d \n", sizeof(byteArray));
  for (size_t i = 0; i < sizeof(unsigned long); i++) {
    // Serial.printf("%d ", ((value >> (i * 8)) & 0xFF));
    byteArray[i] = (value >> (i * 8)) & 0xFF; // Extract each byte
    // Serial.printf("a%d ", byteArray[i]);
    // Serial.printf("b%d \n", (uint8_t)byteArray[i]);
  }
  // Serial.println();
}

unsigned int msgChk(char * buffer, long length) {
  long index;
  unsigned int checksum;

  for (index = 0L, checksum = 0; index < length; checksum += (unsigned int) buffer[index++]);
  return (unsigned int) (checksum % 256);
}

void printFloat(float val, bool valid, int len, int prec) {
  if (!valid) {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
    return;
  }

  Serial.print(val, prec);
  int vi = abs((int)val);
  int flen = prec + (val < 0.0 ? 2 : 1); // . and -
  flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
  for (int i = flen; i < len; ++i)
    Serial.print(' ');
}

void printInt(unsigned long val, bool valid, int len) {
  char sz[32] = "*****************";
  if (val == ULONG_MAX)
    valid = false;
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
}

void printStr(const char *str, int len) {
  int slen = strlen(str);
  for (int i = 0; i < len; ++i)
    Serial.print(i < slen ? str[i] : ' ');
}

float round2dec(float var) {
  // 37.66666 * 100 =3766.66
  // 3766.66 + .5 =3767.16    for rounding off value
  // then type cast to int so value is 3767
  // then divided by 100 so the value converted into 37.67
  float value = (int)(var * 100 + .5);
  return (float)value / 100;
}

long roundUp(float val) {
  long ret = (long)val;
  if (val > ret)
    return ret + 1;
  return ret;
}

String stringPad(const char* value, int width) {
  // Serial.print(F("\nwidth: "));
  // Serial.print(width);
  // Serial.print(F(","));
  // Serial.print(F("value: "));
  // Serial.print(value);
  // Serial.print(F(","));
  int valueLength = strlen(value);
  // Serial.print(F("length: "));
  // Serial.print(valueLength);
  // Serial.print(F(","));
  valueLength = width - valueLength;
  // Serial.print(F("length2: "));
  // Serial.print(valueLength);
  // Serial.print(F(","));
  if (valueLength < 0)
    valueLength = 0;
  if (valueLength > width)
    valueLength = width;
  // Serial.print(F("length3: "));
  // Serial.println(valueLength);
  // Serial.print(F(","));
  
  char blank[valueLength + 1];
  snprintf(blank, sizeof(blank), "%*s", valueLength, ""); 
  snprintf(blank, valueLength, "%*s", valueLength, "");
  // int lengthB = snprintf(blank, valueLength, "%*s", valueLength, "");
  // Serial.print(F("lengthB: "));
  // Serial.print(lengthB);
  // Serial.print(F(","));
  // Serial.print(F("blank: '"));
  // Serial.print(blank);
  // Serial.print(F("'"));
  return blank;
}

String stringPad(float value, int width, char * format) {
  char buffer[40];
  // Serial.print(F("\nwidth: "));
  // Serial.print(width);
  // Serial.print(F(","));
  // Serial.print(F("value: "));
  // Serial.print(value);
  // Serial.print(F(","));
  // Convert float to string
  sprintf(buffer, format, value); 
  // Serial.print(F("buffer: "));
  // Serial.print(buffer);
  // Serial.print(F(","));
  int valueLength = strlen(buffer);
  // Serial.print(F("length: "));
  // Serial.print(valueLength);
  // Serial.print(F(","));
  valueLength = width - valueLength;
  // Serial.print(F("length2: "));
  // Serial.print(valueLength);
  // Serial.print(F(","));
  if (valueLength < 0)
    valueLength = 0;
  if (valueLength > width)
    valueLength = width;
  // Serial.print(F("length3: "));
  // Serial.print(valueLength);
  // Serial.print(F(","));
  
  char blank[valueLength + 1];
  snprintf(blank, valueLength, "%*s", valueLength, "");
  // int lengthB = snprintf(blank, valueLength, "%*s", valueLength, "");
  // Serial.print(F("lengthB: "));
  // Serial.print(lengthB);
  // Serial.print(F(","));
  // Serial.print(F("blank: '"));
  // Serial.print(blank);
  // Serial.print(F("'"));
  return blank;
}

String stringPad(int value, int width) {
  char buffer[40];
  // Serial.print(F("\nwidth: "));
  // Serial.print(width);
  // Serial.print(F(","));
  // Serial.print(F("value: "));
  // Serial.print(value);
  // Serial.print(F(","));
  // Convert int to string
  sprintf(buffer, "%d", value); 
  // Serial.print(F("buffer: "));
  // Serial.print(buffer);
  // Serial.print(F(","));
  int valueLength = strlen(buffer);
  // Serial.print(F("length: "));
  // Serial.print(valueLength);
  // Serial.print(F(","));
  valueLength = width - valueLength;
  // Serial.print(F("length2: "));
  // Serial.print(valueLength);
  // Serial.print(F(","));
  if (valueLength < 0)
    valueLength = 0;
  if (valueLength > width)
    valueLength = width;
  // Serial.print(F("length3: "));
  // Serial.println(valueLength);
  // Serial.print(F(","));
  
  char blank[valueLength + 1];
  snprintf(blank, valueLength, "%*s", valueLength, "");
  // int lengthB = snprintf(blank, valueLength, "%*s", valueLength, "");
  // Serial.print(F("lengthB: "));
  // Serial.print(lengthB);
  // Serial.print(F(","));
  // Serial.print(F("blank: '"));
  // Serial.print(blank);
  // Serial.print(F("'"));
  return blank;
}