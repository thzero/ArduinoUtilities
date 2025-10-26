#include <Arduino.h>

#include <utilities.h>

#include "communicationConstants.h"
#include "communicationSerial.h"

CommunicationSerial::CommunicationSerial() {
}

#if defined(TEENSYDUINO)
bool CommunicationSerial::setup(HardwareSerial& serial, unsigned long baud, uint32_t config, uint8_t* bufferRead, size_t bufferReadLen, uint8_t* bufferWrite, size_t bufferWriteLen) {
  // Serial2.begin(baud, config); // TODO: Would like to pass in the actual serial...
  // while(!Serial2);
  // Serial2.setTimeout(10);
  serial.begin(baud, config);
  while(!serial);
  serial.setTimeout(10);
  Serial.println(F("\tEnabled serial..."));

#if defined(TEENSYDUINO)
  if (bufferRead != nullptr && bufferWrite != nullptr) {
    // uint8_t bigserialbuffer[1024];
    // Serial2.addMemoryForRead(&bigserialbuffer, sizeof(bigserialbuffer));
    HardwareSerialIMXRT *serialIMXRT = static_cast<HardwareSerialIMXRT*>(&serial);
    serialIMXRT->addMemoryForRead(bufferRead, bufferReadLen);
    serialIMXRT->addMemoryForWrite(bufferWrite, bufferWriteLen);
  }
#endif
  Serial.println(F("\t...enabled serial."));

  Serial.println(F("\tEnable transfer on serial."));
#ifdef DEBUG_COMMUNICATION_SERIAL_INTERNAL
  uint8_t debug = 2;
#else
  uint8_t debug = 0;
#endif

  _setup(serial, debug);
  Serial.println(F("\t...enabled transfer on serial."));

  return true;
}
#endif

#if !defined(TEENSYDUINO)
bool CommunicationSerial::setup(HardwareSerial& serial, unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, size_t bufferLen) {
  serial.begin(baud, config, rxPin, txPin);
  while(!serial);
  serial.setTimeout(10);
  Serial.printf(F("\tEnabled serial on rx %d, tx %d...\n"), rxPin, txPin);

#if defined(EPS32)
  // Serial2.setRxBufferSize(1024);
  serial.setRxBufferSize(bufferLen);
#endif
  Serial.println(F("\t...enabled serial."));

  Serial.println(F("\tEnable transfer on serial."));
#ifdef DEBUG_COMMUNICATION
  uint8_t debug = 2;
#else
  uint8_t debug = 0;
#endif

  _setup(serial, debug);
  Serial.println(F("\t...enabled transfer on serial."));

  return true;
}
#endif

CommunicationCommandFunctionPtr CommunicationSerial::_getCommandFunction(uint16_t command) {
#ifdef DEBUG_COMMUNICATION_SERIAL_COMMAND
  Serial.printf(F("_getCommandFunction.command: "));
  Serial.println(command);
  // for (const auto& pair : _commands) {
  //   Serial.printf(pair.second->command);
  //   Serial.println();
  // }
#endif

  auto results = _commands.find(command);
  if (results != _commands.end()) {
#ifdef DEBUG_COMMUNICATION_SERIAL_COMMAND
    Serial.printf(F("_getCommandFunction...found '%d'\n"), command);
#endif
    CommunicationCommandFunctionEntry *current = _commands.at(command);
    if (current != nullptr) {
#ifdef DEBUG_COMMUNICATION_SERIAL_COMMAND
      Serial.printf(F("_getCommandFunction...found '%d'\n"), command);
#endif
      return current->func;
    }
  }
  else
    Serial.printf(F("_getCommandFunction...did not find  '%d'\n"), command);

  return NULL; // command not found
}

bool CommunicationSerial::healthy() {
  return _health;
}

void CommunicationSerial::initCommand(uint16_t command, CommunicationCommandFunctionPtr func) {
  CommunicationCommandFunctionEntry *item = (CommunicationCommandFunctionEntry *)malloc(sizeof(CommunicationCommandFunctionEntry));
  item->command = command;
  item->func = func;
  item->next = NULL;

  Serial.printf(F("\t\tInitializing command: %d\n"), command);

  _commands.insert(std::make_pair(command, item));
}

size_t CommunicationSerial::read(CommunicationHandlerFunctionPtr func, unsigned long timestamp, unsigned long delta) {
  CommuicationMessageStruct communication;
  bool available = _read(&communication, timestamp, delta);
  if (!available)
    return 0;

#ifdef DEBUG_COMMUNICATION_SERIAL_READ
  Serial.printf(F("communication-serial-read: message bytes received: %d\n"), communication.size);
  Serial.printf(F("communication-serial-read: message bytes received: "));
  for (size_t i = 0; i < communication.size; i++)
      Serial.printf(F("%d "), communication.buffer[i]);
  Serial.println();
#endif

  if (communication.command == COMMAND_COMMUNICATION_HEALTH) {
#ifdef DEBUG_COMMUNICATION_READ_HEALTH
    Serial.println(F("communication-serial-read: received health request..."));
#endif
    _health = true;
    _healthMs = millis();
    return 0;
  }

  // if health checks haven't occured, then no other processing...
  if (!_health)
    return 0;

  CommunicationHandlerFunctionPtr commandFunc = _getCommandFunction(communication.command);

  if (commandFunc != nullptr)
    commandFunc(timestamp, delta, communication);
  else if (func != nullptr)
    func(timestamp, delta, communication);

  // TODO: read as many sent messages as are in the buffer using the START_BYTE as demarkation

  return communication.size;
}

int CommunicationSerial::send(uint16_t command) {
#ifdef DEBUG_COMMUNICATION_SERIAL_SEND
  Serial.printf(F("communication-serial-send: message command: "));
  Serial.printf(F("%d\n"), command);
#endif

  CommuicationMessageStruct message;
  uint8_t byteArray[1];
  byteArray[1] = 0;
  memcpy(message.buffer, byteArray, 1);
  message.size = 1;
  message.command = command;

#ifdef DEBUG_COMMUNICATION_SERIAL_SEND
  Serial.printf(F("communication-serial-send: message command to send: %d\n"), message.command);
  Serial.printf(F("communication-serial-send: message size to send: %d\n"), message.size);
  Serial.println(F("communication-serial-send: message bytes: "));
  for (size_t i = 0; i < message.size; i++)
      Serial.printf(F("%d "), message.buffer[i]);
  Serial.println();
  Serial.println(F("communication-serial-send: send"));
#endif
  
  // uint16_t sendSize = _transfer.txObj(message.buffer, 0, message.size);
  // _transfer.sendData(sendSize, message.command);

  // return sendSize;
  return _send(message);
}

bool CommunicationSerial::_read(CommuicationMessageStruct* communication, unsigned long timestamp, unsigned long delta) {
  return false;
}

int CommunicationSerial::_send(CommuicationMessageStruct& message) {
  return 0;
}

bool CommunicationSerial::_setup(HardwareSerial& serial, bool debug) {
  return false;
}