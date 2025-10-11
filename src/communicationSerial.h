#ifndef _COMMMUNICATION_SERIAL_H
#define _COMMMUNICATION_SERIAL_H

#include <Arduino.h>
#include <SerialTransfer.h>

#include <CircularQueue.hpp>
#include <map>

#include "communicationData.h"

#define COMMUNICATION_QUEUE_LENGTH 5
// #define DEBUG_COMMUNICATION
// #define DEBUG_COMMUNICATION_COMMAND
// #define DEBUG_COMMUNICATION_INTERNAL
// #define DEBUG_COMMUNICATION_PROCESS
// #define DEBUG_COMMUNICATION_QUEUE
// #define DEBUG_COMMUNICATION_READ
// #define DEBUG_COMMUNICATION_SEND

class CommunicationSerial {
  public:
    CommunicationSerial();
    void initCommand(uint16_t key, CommunicationCommandFunctionPtr func);
    int process(unsigned long timestamp, unsigned long delta);
    int queue(uint16_t command);
    int queue(uint16_t command, uint8_t *byteArray, size_t size);
    int send(uint16_t command);
    bool healthy();

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

    bool _health = false;
    uint16_t _healthMs = 0;
};

extern CommunicationSerial _communicationSerialObj;

#endif