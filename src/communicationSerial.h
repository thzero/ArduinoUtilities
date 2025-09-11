#ifndef _COMMMUNICATION_SERIAL_H
#define _COMMMUNICATION_SERIAL_H

#include <Arduino.h>

extern int communicationQueue(uint8_t *byteArray, size_t size);
extern int communicationQueueLoop(unsigned long delta);
extern bool communicationQueueSerialSetup();

#endif