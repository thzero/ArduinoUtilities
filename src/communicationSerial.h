#ifndef _COMMMUNICATION_SERIAL_H
#define _COMMMUNICATION_SERIAL_H

#include <map>

#include "communicationData.h"

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
    size_t read(CommunicationHandlerFunctionPtr func, unsigned long timestamp, unsigned long delta);
    int send(uint16_t command);
    
  protected:
    virtual bool _setup(HardwareSerial& serial, bool debug);
    virtual bool _read(CommuicationMessageStruct* communication, unsigned long timestamp, unsigned long delta);
    virtual int _send(CommuicationMessageStruct& message);

    bool _health = false;
    uint16_t _healthMs = 0;
    
  private:
    CommunicationCommandFunctionPtr _getCommandFunction(uint16_t command);

    std::map<uint16_t, CommunicationCommandFunctionEntry*> _commands;
};

#endif