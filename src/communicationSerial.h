#ifndef _COMMMUNICATION_SERIAL_H
#define _COMMMUNICATION_SERIAL_H

#include <Arduino.h>
#include <SerialTransfer.h>

#include <CircularQueue.hpp>
#include <map>

#define COMMUNICATION_QUEUE_LENGTH 5
#define BUFFER_MAX_SIZE 1024
#define BUFFER_MAX_MESSAGE_SIZE 1021 // start - command - size - size - bytes - crc - end

const uint8_t MESSAGE_START_BYTE = 0x7E;
const uint8_t MESSAGE_STOP_BYTE  = 0x81;

struct CommuicationQueueMessageStruct {
  uint16_t command;
  size_t size;
  uint8_t buffer[BUFFER_MAX_MESSAGE_SIZE];
  uint8_t crc;
};

typedef void (*CommunicationCommandFunctionPtr)(unsigned long timestamp, unsigned long deltaElapsed, CommuicationQueueMessageStruct communication);
typedef void (*CommunicationHandlerFunctionPtr)(unsigned long timestamp, unsigned long deltaElapsed, CommuicationQueueMessageStruct communication);

struct CommunicationCommandFunctionEntry {
    uint16_t command;
    CommunicationCommandFunctionPtr func;
    struct CommunicationCommandFunctionEntry *next;
};

class CommunicationSerial {
  public:
    CommunicationSerial();
    void initCommand(uint16_t key, CommunicationCommandFunctionPtr func);
    int loop(unsigned long timestamp, unsigned long delta);
    int queue(uint16_t command);
    int queue(uint16_t command, uint8_t *byteArray, size_t size);

    template <typename T>
    // int queuePacked(uint16_t command, const T& val, const uint16_t& size = sizeof(T));
    int queuePacked(uint16_t command, const T& val, const uint16_t& size = sizeof(T)) {
      // convert packaged struct into byte array
      uint8_t byteArray[size];
      // uint8_t* ptr = (uint8_t*)&val;
      // for (uint16_t i = index; i < size; i++) {
      //   byteArray[i] = *ptr;
      //   ptr++;
      // }
      memcpy(byteArray, &val, size);

      return queue(command, byteArray, size);
    }

    size_t read(CommunicationHandlerFunctionPtr func, unsigned long timestamp, unsigned long delta);
    bool setup(unsigned long baud, uint32_t config);
#if !defined(TEENSYDUINO)
    bool setup(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin);
#endif
    
  private:
    CommunicationCommandFunctionPtr getCommandFunction(uint16_t command);

    // CommunicationCommandFunctionEntry *commandsLatest;
    // CommunicationCommandFunctionEntry *commandsHead;
    std::map<uint16_t, CommunicationCommandFunctionEntry*> _commands;
    CircularQueue<CommuicationQueueMessageStruct, COMMUNICATION_QUEUE_LENGTH> _queueing;
    SerialTransfer _transfer;
};

extern CommunicationSerial _communicationSerialObj;

#endif