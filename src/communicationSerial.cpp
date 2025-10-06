#include <Arduino.h>
#if defined(ESP32)
#include <mutex>
#endif
#if defined(TEENSYDUINO)
#include <TeensyThreads.h>
#endif

#include "communicationSerial.h"
#include <utilities.h>

#if defined(ESP32)
SemaphoreHandle_t mutex;
// std::mutex serial_mtx;
#endif
#if defined(TEENSYDUINO)
Threads::Mutex mutexOutput;
#endif

CommunicationSerial::CommunicationSerial() {
}

CommunicationCommandFunctionPtr CommunicationSerial::getCommandFunction(uint16_t command) {
  // if (commandsHead == NULL)
  //   return NULL;

#ifdef DEBUG_COMMUNICATION
  Serial.printf(F("getCommandFunction.command: "));
  Serial.println(command);
  // for (const auto& pair : _commands) {
  //   Serial.printf(pair.second->command);
  //   Serial.println();
  // }
#endif

  auto results = _commands.find(command);
  if (results != _commands.end()) {
#ifdef DEBUG_COMMUNICATION
    Serial.printf(F("getCommandFunction...found '%d'\n"), command);
#endif
    CommunicationCommandFunctionEntry *current = _commands.at(command);
    if (current != nullptr) {
#ifdef DEBUG_COMMUNICATION
      Serial.printf(F("getCommandFunction...found '%d'\n"), command);
#endif
      return current->func;
    }
  }
#ifdef DEBUG_COMMUNICATION
  else
    Serial.printf(F("getCommandFunction...did not find  '%d'\n"), command);
#endif
      
  // CommunicationCommandFunctionEntry *current = commandsHead;
  // while (current != NULL) {
  //   // Serial.printf(F("command.current: "));
  //   // Serial.println(current->command);
  //     if (current->command != command) {
  //       current = current->next;
  //       continue;
  //     }

  //     return current->func;
  // }

  return NULL; // command not found
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

  _commands.insert(std::make_pair(command, item));
}

int CommunicationSerial::process(unsigned long timestamp, unsigned long delta) {
#if defined(TEENSYDUINO)
  // Threads::Scope m(mutexOutput); // lock on creation
  mutexOutput.lock();
#endif
#if defined(ESP32)
  if (xSemaphoreTake(mutex, portMAX_DELAY)) {
#endif

  if (_queueing.isEmpty()) {
    // Serial.println(F("communication-serial-process: nothing to send."));
#if defined(TEENSYDUINO)
    mutexOutput.unlock();
#endif

#if defined(ESP32)
    xSemaphoreGive(mutex); // Release the mutex
#endif
    return 2; // nothing to send.
  }

// TODO: stuff as many queued messages as will fit in the BUFFER_MAX_SIZE buffer using the START_BYTE as demarkation

  CommuicationQueueMessageStruct message = _queueing.front();
  _queueing.dequeue();

#ifdef DEBUG_COMMUNICATION
  Serial.printf(F("communication-serial-process: message command to send: %d\n"), message.command);
  Serial.printf(F("communication-serial-process: message bytes size to send: %d\n"), message.size);

  Serial.println(F("communication-serial-process: message bytes to send: "));
  for (size_t i = 0; i < message.size; i++)
      Serial.printf(F("%d "), message.buffer[i]);
  Serial.println();
#endif

  // Serial2.write(message.buffer, message.size);

  // uint16_t sendSize = 0;
  // sendSize = _transfer.txObj(message.command, sendSize);
  // sendSize = _transfer.txObj(message.size, sendSize);
  // sendSize = _transfer.txObj(message.buffer, 0, message.size); // Stuff buffer with arra
  uint16_t sendSize = _transfer.txObj(message.buffer, 0, message.size); // Stuff buffer with array
  // Serial.printf(F("communication-serial-process: message bytes sent: "));
  // Serial.printf(F("%d\n"), sendSize);

  _transfer.sendData(sendSize, message.command);

  memset(message.buffer, 0, BUFFER_MAX_SIZE);

#ifdef DEBUG_COMMUNICATION
  Serial.println(F("communication-serial-process: sent buffer."));
#endif

#if defined(ESP32)
    xSemaphoreGive(mutex); // Release the mutex
  }
#endif
#if defined(TEENSYDUINO)
  mutexOutput.unlock();
#endif

  return 1;
}

int CommunicationSerial::queue(uint16_t command) {
#ifdef DEBUG_COMMUNICATION
  Serial.printf(F("communication-serial-queue: message command: "));
  Serial.printf(F("%d\n"), command);
#endif

#if defined(TEENSYDUINO)
  // Threads::Scope m(mutexOutput); // lock on creation
  mutexOutput.lock();
#endif
#if defined(ESP32)
  if (xSemaphoreTake(mutex, portMAX_DELAY)) {
#endif

  if (_queueing.size() >= COMMUNICATION_QUEUE_LENGTH) {
    Serial.println(F("communication-serial-queue: all queues are full."));
    
#if defined(ESP32)
    xSemaphoreGive(mutex); // Release the mutex
#endif
#if defined(TEENSYDUINO)
    mutexOutput.unlock();
#endif

    return -2; // exceeded queue length
  }

  CommuicationQueueMessageStruct message;
  uint8_t byteArray[1];
  byteArray[1] = 0;
  memcpy(message.buffer, byteArray, 1);
  message.size = 1;
  message.command = command;

#ifdef DEBUG_COMMUNICATION
  Serial.printf(F("communication-serial-queue: message command to send: %d\n"), message.command);
  Serial.printf(F("communication-serial-queue: message size to send: %d\n"), message.size);
  Serial.println(F("communication-serial-queue: message bytes: "));
  for (size_t i = 0; i < message.size; i++)
      Serial.printf(F("%d "), message.buffer[i]);
  Serial.println();
#endif

  _queueing.enqueue(message);

#ifdef DEBUG_COMMUNICATION
  Serial.println(F("communication-serial-queue: message queued."));
#endif

#if defined(ESP32)
    xSemaphoreGive(mutex); // Release the mutex
  }
#endif
#if defined(TEENSYDUINO)
  mutexOutput.unlock();
#endif

  return 1;
}

int CommunicationSerial::queue(uint16_t command, uint8_t *byteArray, size_t size) {
  if (size > BUFFER_MAX_MESSAGE_SIZE) {
    Serial.printf(F("communication-serial-queue: message is too long, max size is %d."), BUFFER_MAX_SIZE);
    return -1; // exceeded queue length
  }

#ifdef DEBUG_COMMUNICATION
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

#ifdef DEBUG_COMMUNICATION
  Serial.printf(F("communication-serial-queue: message command to send: %d\n"), message.command);
  Serial.printf(F("communication-serial-queue: message size to send: %d\n"), message.size);
  Serial.println(F("communication-serial-queue: message bytes: "));
  for (size_t i = 0; i < message.size; i++)
      Serial.printf(F("%d "), message.buffer[i]);
  Serial.println();
#endif

#if defined(TEENSYDUINO)
  // Threads::Scope m(mutexOutput); // lock on creation
  mutexOutput.lock();
#endif
#if defined(ESP32)
  if (xSemaphoreTake(mutex, portMAX_DELAY)) {
#endif

  if (_queueing.size() >= COMMUNICATION_QUEUE_LENGTH) {
    Serial.println(F("communication-serial-queue: all queues are full."));

#if defined(ESP32)
    xSemaphoreGive(mutex); // Release the mutex
#endif
#if defined(TEENSYDUINO)
    mutexOutput.unlock();
#endif

    return -2; // exceeded queue length
  }

  _queueing.enqueue(message);

#ifdef DEBUG_COMMUNICATION
  Serial.println(F("communication-serial-queue: message queued."));
#endif

#if defined(ESP32)
    xSemaphoreGive(mutex); // Release the mutex
  }
#endif
#if defined(TEENSYDUINO)
  mutexOutput.unlock();
#endif

  return 1;
}

size_t CommunicationSerial::read(CommunicationHandlerFunctionPtr func, unsigned long timestamp, unsigned long delta) {
  if (_transfer.available()) {
    CommuicationQueueMessageStruct communication;
    // uint8_t command;
    // uint8_t buffer[BUFFER_MAX_SIZE];
    // size_t size;
    communication.command = _transfer.currentCommand();
#ifdef DEBUG_COMMUNICATION
    Serial.printf(F("communication-serial-read: message command received: %d %d %d\n"), communication.command, _transfer.currentCommand(), (communication.command == _transfer.currentCommand()));
#endif
    communication.size = _transfer.currentReceived();
#ifdef DEBUG_COMMUNICATION
    Serial.printf(F("communication-serial-read: message bytes to receive: %d\n"), communication.size);
#endif
//     uint16_t recSize = 0;  // bytes we've processed from the receive buffer
//     recSize = _transfer.rxObj(communication.size, sizeof(size_t));
#ifdef DEBUG_COMMUNICATION
    Serial.printf(F("communication-serial-read: message bytes to receive: %d\n"), communication.size);
#endif
    // recSize = _transfer.rxObj(communication.buffer, recSize, communication.size);
    uint16_t recSize = _transfer.rxObj(communication.buffer, 0, communication.size);
#ifdef DEBUG_COMMUNICATION
    Serial.printf(F("communication-serial-read: message bytes to received: %d\n"), recSize);
    Serial.printf(F("communication-serial-read: message bytes received: "));
    for (size_t i = 0; i < communication.size; i++)
        Serial.printf(F("%d "), communication.buffer[i]);
    Serial.println();
#endif

    CommunicationHandlerFunctionPtr commandFunc = getCommandFunction(communication.command);

    if (commandFunc != nullptr)
      commandFunc(timestamp, delta, communication);
    else if (func != nullptr)
      func(timestamp, delta, communication);

    // TODO: read as many sent messages as are in the buffer using the START_BYTE as demarkation

    return communication.size;
  }

//   if (Serial2.available() > 0) { // Check if data is available
//     //  Serial.println(F("Serial2.available..."));
//     CommuicationQueueMessageStruct communication;

//     communication.size = Serial2.readBytesUntil(';', communication.buffer, BUFFER_MAX_SIZE - 1);
    
// #ifdef DEBUG_COMMUNICATION
//     Serial.printf(F("communication-serial-read: message size received: %d\n"), communication.size);
// #endif
//     // recSize = _transfer.rxObj(communication.buffer, recSize, communication.size);

// #ifdef DEBUG_COMMUNICATION
//     Serial.println(F("communication-serial-read: message bytes to receive: "));
//     for (int i = 0; i < communication.size; i++)
//       Serial.printf(F(%d "), communication.buffer[i]);
//     Serial.println();
// #endif

  //   if (func != nullptr)
  //       func(timestamp, delta, communication);

  //   return communication.size;
  // }
  return 0;
}

bool CommunicationSerial::setup(unsigned long baud, uint32_t config) {
  Serial2.begin(baud, config); // TODO: Would like to pass in the actual serial...
  // Serial2.begin(1000000);
  while(!Serial2);
  Serial.println(F("\tEnabled Serial2..."));
  Serial2.setTimeout(10);
#if defined(EPS32)
  Serial2.setRxBufferSize(1024);
#endif
#if defined(TEENSYDUINO)
uint8_t bigserialbuffer[1024];
Serial1.addMemoryForRead(&bigserialbuffer, sizeof(bigserialbuffer));
#endif
  Serial.println(F("\t...enabled Serial2."));

  Serial.println(F("\tEable transfer on Serial2."));
#ifdef DEBUG_COMMUNICATION
  uint8_t debug = 2;
#else
  uint8_t debug = 0;
#endif
  _transfer.begin(Serial2, debug);
  Serial.println(F("\t...enabled transfer on Serial2."));

#if defined(ESP32)
  mutex = xSemaphoreCreateMutex();
  if (mutex == NULL) {
    Serial.println(F("Failed to create mutex"));
    return false;
  }
#endif

  return true;
}

#if !defined(TEENSYDUINO)
bool CommunicationSerial::setup(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin) {
  Serial2.begin(baud, config, rxPin, txPin);
  while(!Serial2);
  Serial.println(F("\tEnabled Serial2..."));
  Serial2.setTimeout(10);
#if defined(EPS32)
  Serial2.setRxBufferSize(1024);
#endif
  Serial.println(F("\t...enabled Serial2."));

  Serial.println(F("\tEnable transfer on Serial2."));
#ifdef DEBUG_COMMUNICATION
  uint8_t debug = 2;
#else
  uint8_t debug = 0;
#endif
  _transfer.begin(Serial2, debug);
  Serial.println(F("\t...enabled transfer on Serial2."));

#if defined(ESP32)
  mutex = xSemaphoreCreateMutex();
  if (mutex == NULL) {
    Serial.println(F("Failed to create mutex"));
    return false;
  }
#endif

  return true;
}
#endif

CommunicationSerial _communicationSerialObj;