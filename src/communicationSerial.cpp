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

  // _transfer.begin(Serial2, debug);
  // _transfer.begin(serial, debug);
  _setup(serial, debug);
  Serial.println(F("\t...enabled transfer on serial."));

  return true;
}
#endif

#if !defined(TEENSYDUINO)
bool CommunicationSerial::setup(HardwareSerial& serial, unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, size_t bufferLen) {
  // Serial2.begin(baud, config, rxPin, txPin);
  // while(!Serial2);
  // Serial.println(F("\tEnabled Serial2..."));
  // Serial2.setTimeout(10);
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

  // _transfer.begin(Serial2, debug);
  // _transfer.begin(serial, debug);
  _setup(serial, debug);
  Serial.println(F("\t...enabled transfer on serial."));

#if defined(ESP32)
  _communicationSerialMutexOutput = xSemaphoreCreateMutex();
  if (_communicationSerialMutexOutput == NULL) {
    Serial.println(F("Failed to create mutex"));
    return false;
  }
#endif

  return true;
}
#endif

CommunicationCommandFunctionPtr CommunicationSerial::getCommandFunction(uint16_t command) {
#ifdef DEBUG_COMMUNICATION_SERIAL_COMMAND
  Serial.printf(F("getCommandFunction.command: "));
  Serial.println(command);
  // for (const auto& pair : _commands) {
  //   Serial.printf(pair.second->command);
  //   Serial.println();
  // }
#endif

  auto results = _commands.find(command);
  if (results != _commands.end()) {
#ifdef DEBUG_COMMUNICATION_SERIAL_COMMAND
    Serial.printf(F("getCommandFunction...found '%d'\n"), command);
#endif
    CommunicationCommandFunctionEntry *current = _commands.at(command);
    if (current != nullptr) {
#ifdef DEBUG_COMMUNICATION_SERIAL_COMMAND
      Serial.printf(F("getCommandFunction...found '%d'\n"), command);
#endif
      return current->func;
    }
  }
  else
    Serial.printf(F("getCommandFunction...did not find  '%d'\n"), command);

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

  // if (commandsHead == NULL)
  //   commandsHead = item;
  // if (commandsLatest != NULL)
  //   commandsLatest->next = item;
  // commandsLatest = item;

  Serial.printf(F("\t\tInitializing command: %d\n"), command);

  _commands.insert(std::make_pair(command, item));
}

int CommunicationSerial::process(unsigned long timestamp, unsigned long delta) {
  if (_health == 1) {
    uint16_t now = millis();
    uint16_t delta = now - _healthMs;
    if (delta > 5000) {
      _health = 0;
      _healthMs = 0;
    }
  }

#if defined(TEENSYDUINO)
  // Threads::Scope m(_communicationSerialMutexOutput); // lock on creation
  _communicationSerialMutexOutput.lock();
#endif
#if defined(ESP32)
  if (xSemaphoreTake(_communicationSerialMutexOutput, portMAX_DELAY)) {
#endif

  if (_queueing.isEmpty()) {
    // Serial.println(F("communication-serial-process: nothing to send."));
#if defined(TEENSYDUINO)
    _communicationSerialMutexOutput.unlock();
#endif

#if defined(ESP32)
    xSemaphoreGive(_communicationSerialMutexOutput); // Release the mutex
#endif
    return 2; // nothing to send.
  }

// TODO: stuff as many queued messages as will fit in the BUFFER_MAX_SIZE buffer using the START_BYTE as demarkation

  CommuicationQueueMessageStruct message = _queueing.front();
  _queueing.dequeue();

#ifdef DEBUG_COMMUNICATION_SERIAL_PROCESS
  Serial.printf(F("communication-serial-process: message command to send: %d\n"), message.command);
  Serial.printf(F("communication-serial-process: message bytes size to send: %d\n"), message.size);

  Serial.println(F("communication-serial-process: message bytes to send: "));
  for (size_t i = 0; i < message.size; i++)
      Serial.printf(F("%d "), message.buffer[i]);
  Serial.println();
#endif

  // // uint16_t sendSize = 0;
  // // sendSize = _transfer.txObj(message.command, sendSize);
  // // sendSize = _transfer.txObj(message.size, sendSize);
  // // sendSize = _transfer.txObj(message.buffer, 0, message.size); // Stuff buffer with arra
  // uint16_t sendSize = _transfer.txObj(message.buffer, 0, message.size); // Stuff buffer with array
  // // Serial.printf(F("communication-serial-process: message bytes sent: "));
  // // Serial.printf(F("%d\n"), sendSize);

  // _transfer.sendData(sendSize, message.command);
  _process(message, timestamp, delta);

  memset(message.buffer, 0, BUFFER_MAX_SIZE);

#ifdef DEBUG_COMMUNICATION_SERIAL_PROCESS
  Serial.println(F("communication-serial-process: sent buffer."));
#endif

#if defined(ESP32)
    xSemaphoreGive(_communicationSerialMutexOutput); // Release the mutex
  }
#endif
#if defined(TEENSYDUINO)
  _communicationSerialMutexOutput.unlock();
#endif

  return 1;
}

int CommunicationSerial::queue(uint16_t command) {
  if (!_health) {
#ifdef DEBUG_COMMUNICATION_SERIAL_QUEUE
    Serial.printf(F("communication-serial-queue: health is %d.\n"), _health);
#endif
    return 0;
  }

#ifdef DEBUG_COMMUNICATION_SERIAL_QUEUE
  Serial.printf(F("communication-serial-queue: message command: "));
  Serial.printf(F("%d\n"), command);
#endif

#if defined(TEENSYDUINO)
  // Threads::Scope m(_communicationSerialMutexOutput); // lock on creation
  _communicationSerialMutexOutput.lock();
#endif
#if defined(ESP32)
  if (xSemaphoreTake(_communicationSerialMutexOutput, portMAX_DELAY)) {
#endif

  if (_queueing.size() >= COMMUNICATION_QUEUE_LENGTH) {
    Serial.println(F("communication-serial-queue: all queues are full."));
    
#if defined(ESP32)
    xSemaphoreGive(_communicationSerialMutexOutput); // Release the mutex
#endif
#if defined(TEENSYDUINO)
    _communicationSerialMutexOutput.unlock();
#endif

    return -2; // exceeded queue length
  }

  CommuicationQueueMessageStruct message;
  uint8_t byteArray[1];
  byteArray[1] = 0;
  memcpy(message.buffer, byteArray, 1);
  message.size = 1;
  message.command = command;

#ifdef DEBUG_COMMUNICATION_SERIAL_QUEUE
  Serial.printf(F("communication-serial-queue: message command to send: %d\n"), message.command);
  Serial.printf(F("communication-serial-queue: message size to send: %d\n"), message.size);
  Serial.println(F("communication-serial-queue: message bytes: "));
  for (size_t i = 0; i < message.size; i++)
      Serial.printf(F("%d "), message.buffer[i]);
  Serial.println();
#endif

  _queueing.enqueue(message);

#ifdef DEBUG_COMMUNICATION_SERIAL_QUEUE
  Serial.println(F("communication-serial-queue: message queued."));
#endif

#if defined(ESP32)
    xSemaphoreGive(_communicationSerialMutexOutput); // Release the mutex
  }
#endif
#if defined(TEENSYDUINO)
  _communicationSerialMutexOutput.unlock();
#endif

  return 1;
}

int CommunicationSerial::queue(uint16_t command, uint8_t *byteArray, size_t size) {
  if (!_health) {
#ifdef DEBUG_COMMUNICATION_SERIAL_QUEUE
    Serial.printf(F("communication-serial-queue: health is %d.\n"), _health);
#endif
    return 0;
  }

  if (size > BUFFER_MAX_MESSAGE_SIZE) {
    Serial.printf(F("communication-serial-queue: message is too long, max size is %d."), BUFFER_MAX_SIZE);
    return -1; // exceeded queue length
  }

#ifdef DEBUG_COMMUNICATION_SERIAL_QUEUE
  Serial.printf(F("communication-serial-queue: message command: %d\n"), command);
  Serial.println(F("communication-serial-queue: requested bytes: "));
  for (size_t i = 0; i < size; i++)
      Serial.printf(F("%d "), byteArray[i]);
  Serial.println();
#endif

  CommuicationQueueMessageStruct message;
  memcpy(message.buffer, byteArray, size);
  message.size = size;
  message.command = command;

#ifdef DEBUG_COMMUNICATION_SERIAL_QUEUE
  Serial.printf(F("communication-serial-queue: message command to send: %d\n"), message.command);
  Serial.printf(F("communication-serial-queue: message size to send: %d\n"), message.size);
  Serial.println(F("communication-serial-queue: message bytes: "));
  for (size_t i = 0; i < message.size; i++)
      Serial.printf(F("%d "), message.buffer[i]);
  Serial.println();
#endif

#if defined(TEENSYDUINO)
  // Threads::Scope m(_communicationSerialMutexOutput); // lock on creation
  _communicationSerialMutexOutput.lock();
#endif
#if defined(ESP32)
  if (xSemaphoreTake(_communicationSerialMutexOutput, portMAX_DELAY)) {
#endif

  if (_queueing.size() >= COMMUNICATION_QUEUE_LENGTH) {
    Serial.println(F("communication-serial-queue: all queues are full."));

#if defined(ESP32)
    xSemaphoreGive(_communicationSerialMutexOutput); // Release the mutex
#endif
#if defined(TEENSYDUINO)
    _communicationSerialMutexOutput.unlock();
#endif

    return -2; // exceeded queue length
  }

  _queueing.enqueue(message);

#ifdef DEBUG_COMMUNICATION_SERIAL_QUEUE
  Serial.println(F("communication-serial-queue: message queued."));
#endif

#if defined(ESP32)
    xSemaphoreGive(_communicationSerialMutexOutput); // Release the mutex
  }
#endif
#if defined(TEENSYDUINO)
  _communicationSerialMutexOutput.unlock();
#endif

  return 1;
}

size_t CommunicationSerial::read(CommunicationHandlerFunctionPtr func, unsigned long timestamp, unsigned long delta) {
  CommuicationQueueMessageStruct communication;
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

  CommunicationHandlerFunctionPtr commandFunc = getCommandFunction(communication.command);

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

  CommuicationQueueMessageStruct message;
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

int CommunicationSerial::_process(CommuicationQueueMessageStruct& message, unsigned long timestamp, unsigned long delta) {
  return 0;
}

bool CommunicationSerial::_read(CommuicationQueueMessageStruct* communication, unsigned long timestamp, unsigned long delta) {
  return false;
}

int CommunicationSerial::_send(CommuicationQueueMessageStruct& message) {
  return 0;
}

bool CommunicationSerial::_setup(HardwareSerial& serial, bool debug) {
  return false;
}