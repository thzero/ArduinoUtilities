#ifndef _COMMMUNICATION_SERIAL_QUEUE_H
#define _COMMMUNICATION_SERIAL_QUEUE_H

#include <Arduino.h>
#if defined(ESP32)
#include <mutex>
#endif
#if defined(TEENSYDUINO)
#include <TeensyThreads.h>
#endif

#include <CircularQueue.hpp>

#include "communicationData.h"
#include "communicationSerial.h"

#define COMMUNICATION_QUEUE_LENGTH 5
// #define DEBUG_COMMUNICATION_SERIAL
// #define DEBUG_COMMUNICATION_SERIAL_COMMAND
// #define DEBUG_COMMUNICATION_SERIAL_INTERNAL
// #define DEBUG_COMMUNICATION_SERIAL_PROCESS
// #define DEBUG_COMMUNICATION_SERIAL_QUEUE
// #define DEBUG_COMMUNICATION_SERIAL_READ
// #define DEBUG_COMMUNICATION_SERIAL_READ_HEALTH
// #define DEBUG_COMMUNICATION_SERIAL_SEND

class CommunicationSerialQueue: public CommunicationSerial {
  public:
    CommunicationSerialQueue();
    int process(unsigned long timestamp, unsigned long delta);
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
    
  protected:
    virtual int _process(CommunicationMessageStruct& message, unsigned long timestamp, unsigned long delta);
#if !defined(TEENSYDUINO)
    bool _setup(HardwareSerial& serial, bool debug) override;
#endif
    
  private:
    CircularQueue<CommunicationMessageStruct, COMMUNICATION_QUEUE_LENGTH> _queueing;

    #if defined(ESP32)
    SemaphoreHandle_t _communicationSerialMutexOutput;
    // std::mutex serial_mtx;
    #endif
    #if defined(TEENSYDUINO)
    Threads::Mutex _communicationSerialMutexOutput;
    #endif
};

#endif