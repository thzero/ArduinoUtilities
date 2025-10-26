#ifndef _COMMMUNICATION_DATA_H
#define _COMMMUNICATION_DATA_H

#define BUFFER_MAX_SIZE 1024
#define BUFFER_MAX_MESSAGE_SIZE 1021 // start - command - size - size - bytes - crc - end

const uint8_t MESSAGE_START_BYTE = 0x7E;
const uint8_t MESSAGE_STOP_BYTE  = 0x81;

struct CommuicationMessageStruct {
  uint16_t command;
  size_t size;
  uint8_t buffer[BUFFER_MAX_MESSAGE_SIZE];
  uint8_t crc;
};

typedef void (*CommunicationCommandFunctionPtr)(unsigned long timestamp, unsigned long deltaElapsed, CommuicationMessageStruct communication);
typedef void (*CommunicationHandlerFunctionPtr)(unsigned long timestamp, unsigned long deltaElapsed, CommuicationMessageStruct communication);

struct CommunicationCommandFunctionEntry {
    uint16_t command;
    CommunicationCommandFunctionPtr func;
    struct CommunicationCommandFunctionEntry *next;
};

#endif