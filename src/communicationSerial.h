#ifndef _COMMMUNICATION_SERIAL_H
#define _COMMMUNICATION_SERIAL_H

#include <Arduino.h>
// #include <SerialTransfer.h>

#include <CircularQueue.hpp>

#define COMMUNICATION_QUEUE_LENGTH 5
#define BUFFER_MAX_SIZE 1024

struct CommuicationQueueMessageStruct {
  uint8_t buffer[BUFFER_MAX_SIZE];
  size_t size;
  uint8_t command;
};

typedef void (*CommunicationHandlerFunctionPtr)(unsigned long timestamp, unsigned long deltaElapsed, CommuicationQueueMessageStruct communication);

extern int communicationSerialQueue(uint8_t *byteArray, size_t size);
extern int communicationSerialLoop(unsigned long delta);
extern bool communicationSerialSetup();

class CommunicationSerial {
  public:
    CommunicationSerial();
    int queue(uint8_t command, uint8_t *byteArray, size_t size);
    template <typename T>
    int queuePacked(uint8_t command, const T& val, const uint16_t& size = sizeof(T));
    int loop(unsigned long timestamp, unsigned long delta);
    // size_t read(uint8_t* received, unsigned long timestamp, unsigned long delta, CommunicationHandlerFunctionPtr func);
    size_t read(CommunicationHandlerFunctionPtr func, unsigned long timestamp, unsigned long delta);
    bool setup(unsigned long baud, uint32_t config);
#if !defined(TEENSYDUINO)
    bool setup(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin);
#endif
    
  private:
    CircularQueue<CommuicationQueueMessageStruct, COMMUNICATION_QUEUE_LENGTH> _queueing;
    
    uint8_t _bufferSerialInbound[BUFFER_MAX_SIZE];
    // SerialTransfer _transfer;
};

extern CommunicationSerial _communicationSerialObj;

#endif