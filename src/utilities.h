#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <Arduino.h>

extern unsigned int convertByteArrayToUnsignedInt(uint8_t *byteArray);
extern unsigned long convertByteArrayToUnsignedLong(uint8_t *byteArray);
extern void convertPrintByteArrayUnsignedInt(uint8_t *byteArray);
extern void convertPrintByteArrayUnsignedLong(uint8_t *byteArray);
extern void convertPrintUnsignedByteArrayUnsignedLong(uint8_t *byteArray);
extern void convertUnsignedIntToByteArray(unsigned int value, uint8_t *byteArray);
extern void convertUnsignedLongToByteArray(unsigned long value, uint8_t *byteArray);
extern int convertUnsignedByteArrayToInt(uint8_t *byteArray);
extern uint16_t convertUnsignedByteArrayToShort(uint8_t *byteArray);
extern unsigned int convertUnsignedByteArrayToUnsignedInt(uint8_t *byteArray);
extern unsigned long convertUnsignedByteArrayToUnsignedLong(uint8_t *byteArray);
extern uint16_t convertUnsignedByteArrayToUnsignedShort(uint8_t *byteArray);
extern void convertUnsignedIntToUnsignedByteArray(unsigned int value, uint8_t *byteArray);
extern void convertUnsignedLongToUnsignedByteArray(unsigned long value, uint8_t *byteArray);

extern void feedWatchdog();

extern unsigned int msgChk(char * buffer, long length);

void printFloat(float val, bool valid, int len, int prec);
void printInt(unsigned long val, bool valid, int len);
void printStr(const char *str, int len);

extern float round2dec(float var);
extern long roundUp(float val);

extern String stringPad(const char* value, int width);
extern String stringPad(float value, int width, char * format);
extern String stringPad(int value, int width);

#endif