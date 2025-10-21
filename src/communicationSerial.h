#ifndef _COMMMUNICATION_QUEUE_SERIAL_H
#define _COMMMUNICATION_QUEUE_SERIAL_H

#include <Arduino.h>
#if defined(ESP32)
#include <mutex>
#endif
#if defined(TEENSYDUINO)
#include <TeensyThreads.h>
#endif

#include <CircularQueue.hpp>
#include <map>

#include "communicationData.h"

#define COMMUNICATION_QUEUE_LENGTH 5
// #define DEBUG_COMMUNICATION_SERIAL
// #define DEBUG_COMMUNICATION_SERIAL_COMMAND
// #define DEBUG_COMMUNICATION_SERIAL_INTERNAL
// #define DEBUG_COMMUNICATION_SERIAL_PROCESS
// #define DEBUG_COMMUNICATION_SERIAL_QUEUE
// #define DEBUG_COMMUNICATION_SERIAL_READ
// #define DEBUG_COMMUNICATION_SERIAL_READ_HEALTH
// #define DEBUG_COMMUNICATION_SERIAL_SEND

class CommunicationSerial {
  public:
    CommunicationSerial();
#if defined(TEENSYDUINO)
    bool setup(HardwareSerial& serial, unsigned long baud, uint32_t config, uint8_t* bufferRead = nullptr, size_t bufferReadLen = 0, uint8_t* bufferWrite = nullptr, size_t bufferWriteLen = 0);
#endif
#if !defined(TEENSYDUINO)
    bool setup(HardwareSerial& serial, unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, size_t bufferLen = 1024);
#endif
    // bool setup(HardwareSerial& serial, unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, uint8_t* bufferRead = nullptr, size_t bufferReadLen = 0, uint8_t* bufferWrite = nullptr, size_t bufferWriteLen = 0);
    bool healthy();
    void initCommand(uint16_t key, CommunicationCommandFunctionPtr func);
    int process(unsigned long timestamp, unsigned long delta);
    int queue(uint16_t command);
    int queue(uint16_t command, uint8_t *byteArray, size_t size);
    size_t read(CommunicationHandlerFunctionPtr func, unsigned long timestamp, unsigned long delta);
    int send(uint16_t command);

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
    
  protected:
    virtual int _process(CommuicationQueueMessageStruct& message, unsigned long timestamp, unsigned long delta);
    virtual bool _setup(HardwareSerial& serial, bool debug);
    virtual bool _read(CommuicationQueueMessageStruct* communication, unsigned long timestamp, unsigned long delta);
    virtual int _send(CommuicationQueueMessageStruct& message);
    
  private:
    CommunicationCommandFunctionPtr getCommandFunction(uint16_t command);

    std::map<uint16_t, CommunicationCommandFunctionEntry*> _commands;
    CircularQueue<CommuicationQueueMessageStruct, COMMUNICATION_QUEUE_LENGTH> _queueing;

    bool _health = false;
    uint16_t _healthMs = 0;

    #if defined(ESP32)
    SemaphoreHandle_t _communicationSerialMutexOutput;
    // std::mutex serial_mtx;
    #endif
    #if defined(TEENSYDUINO)
    Threads::Mutex _communicationSerialMutexOutput;
    #endif
};

#endif