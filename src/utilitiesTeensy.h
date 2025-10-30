#ifndef _UTILITIES_TEENSY_H
#define _UTILITIES_TEENSY_H

#if defined(TEENSYDUINO)

#include <Arduino.h>
#include <cstdint>
#include <cstddef>

extern byte crashReport();

extern void bToStr(byte x, char res[20], int d);
extern void fToStr(float n, char res[20], int afterpoint);
extern void iToStr(int x, char res[20], int d);
extern int intToStr(int x, char str[], int d);
extern void reverse(char* str, int len);
extern void ulongToStr(unsigned long x, char res[20], int d);

extern void dumpBuffers(const char *title, const uint8_t *buf, size_t start, size_t len);
extern bool verifyAndDumpDifference(const char *a_title, const uint8_t *a_buf, size_t a_size, const char *b_title, const uint8_t *b_buf, size_t b_size);
extern void initializeBuffers(uint8_t *tx, uint8_t *rx, size_t size, size_t offset = 0);

extern void fetchMAC(uint8_t *mac);

#endif

#endif