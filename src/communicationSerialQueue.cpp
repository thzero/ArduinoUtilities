#include <Arduino.h>

#include <utilities.h>

#include "communicationConstants.h"
#include "communicationSerialQueue.h"

CommunicationSerialQueue::CommunicationSerialQueue() {
}

int CommunicationSerialQueue::process(unsigned long timestamp, unsigned long delta) {
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

  CommunicationMessageStruct message = _queueing.front();
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

int CommunicationSerialQueue::queue(uint16_t command) {
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

  CommunicationMessageStruct message;
  uint8_t byteArray[1];
  byteArray[1] = 0;
  memcpy(message.buffer, byteArray, 1);
  message.size = 1;
  message.command = command;

#ifdef DEBUG_COMMUNICATION_SERIAL_QUEUE
  Serial.printf(F("communication-serial-queue: message command to send: %d\n"), message.command);
  Serial.printf(F("communication-serial-queue: message size to queue: %d\n"), message.size);
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

int CommunicationSerialQueue::queue(uint16_t command, uint8_t *byteArray, size_t size) {
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

  CommunicationMessageStruct message;
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

int CommunicationSerialQueue::_process(CommunicationMessageStruct& message, unsigned long timestamp, unsigned long delta) {
  return 0;
}

#if !defined(TEENSYDUINO)
bool CommunicationSerialQueue::_setup(HardwareSerial& serial, bool debug) {
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