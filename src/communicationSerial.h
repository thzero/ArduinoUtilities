#ifndef _COMMMUNICATION_SERIAL_H
#define _COMMMUNICATION_SERIAL_H

#include <Arduino.h>

extern int communicationSerialQueue(uint8_t *byteArray, size_t size);
extern int communicationSerialLoop(unsigned long delta);
extern bool communicationSerialSetup();

#endif