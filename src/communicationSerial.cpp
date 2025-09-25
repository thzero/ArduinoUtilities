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
std::mutex serial_mtx;
#endif
#if defined(TEENSYDUINO)
Threads::Mutex mutexOutput;
#endif

CommunicationSerial::CommunicationSerial() {
}

int CommunicationSerial::loop(unsigned long timestamp, unsigned long delta) {
#if defined(TEENSYDUINO)
  Threads::Scope m(mutexOutput); // lock on creation
#endif
#if defined(ESP32)
  if (xSemaphoreTake(mutex, portMAX_DELAY)) {
#endif

  if (_queueing.isEmpty()) {
    // Serial.println("communication-serial-loop: nothing to send.");
    return 2; // nothing to send.
  }

  // TODO: stuff as many queued messages as will fit in the BUFFER_MAX_SIZE buffer using the START_BYTE as demarkation

  CommuicationQueueMessageStruct message = _queueing.front();
  _queueing.dequeue();

#ifdef DEBUG
  Serial.print("communication-serial-loop: message command size to send: ");
  Serial.printf("%d\n", message.command);
  Serial.print("communication-serial-loop: message bytes size to send: ");
  Serial.printf("%d\n", message.size);
  
  Serial.println("communication-serial-loop: message bytes to send: ");
  for (size_t i = 0; i < message.size; i++)
      Serial.printf("%d ", message.buffer[i]);
  Serial.println();
#endif

  Serial.print("communication-serial-loop: packed message bytes to send: ");
  for (size_t i = 0; i < message.size; i++)
      Serial.printf("%d ", message.buffer[i]);
  Serial.println();

  // uint8_t crcVal = crc.calculate(message.buffer, message.size-1); // account for ending
  // Serial.println("communication-serial-loop: message crc: ");
  // Serial.printf("%d\n", crcVal);

  // Serial2.write(message.buffer, message.size);

  uint16_t sendSize = 0;
  // sendSize = myTransfer.txObj(testStruct, sendSize); // Stuff buffer with struct
  // TODO: buffer is BUFFER_MAX_SIZE, but data size for serialTransfer is 256
  sendSize = _transfer.txObj(message.command, sizeof(uint8_t)); // Stuff buffer with array
  sendSize = _transfer.txObj(message.size, sizeof(size_t)); // Stuff buffer with array
  sendSize = _transfer.txObj(message.buffer, sendSize, message.size); // Stuff buffer with array
  _transfer.sendData(sendSize);

  memset(message.buffer, 0, BUFFER_MAX_SIZE);

#ifdef DEBUG
  Serial.println("communication-serial-loop: sent buffer.");
#endif
  return 1;

#if defined(ESP32)
    xSemaphoreGive(mutex); // Release the mutex
}
#endif
}

int CommunicationSerial::queue(uint8_t command, uint8_t *byteArray, size_t size) {
  if (size > BUFFER_MAX_SIZE) {
    Serial.printf(F("communication-serial-queue: message is too long, max size is %d."), BUFFER_MAX_SIZE);
    return -1; // exceeded queue length
  }

#if defined(TEENSYDUINO)
  Threads::Scope m(mutexOutput); // lock on creation
#endif

// #ifdef DEBUG
  Serial.println("communication-serial-queue: trying to queue.");
  Serial.println("communication-serial-queue: requested bytes: ");
  for (size_t i = 0; i < size; i++)
      Serial.printf("%d ", byteArray[i]);
  Serial.println();
// #endif

  if (_queueing.size() >= COMMUNICATION_QUEUE_LENGTH) {
    Serial.println(F("communication-serial-queue: all queues are full."));
    return -2; // exceeded queue length
  }

  CommuicationQueueMessageStruct message;
  memcpy(message.buffer, byteArray, size);
  message.size = size;
  message.command = command;

// #ifdef DEBUG
  Serial.println("communication-serial-queue: message bytes: ");
  for (size_t i = 0; i < message.size; i++)
      Serial.printf("%d ", message.buffer[i]);
  Serial.println();
// #endif

  _queueing.enqueue(message);

#ifdef DEBUG
  Serial.println("communication-serial-queue: message queued.");
#endif
  return 1;
}

template <typename T>
int CommunicationSerial::queuePacked(uint8_t command, const T& val, const uint16_t& size) {
  // convert packaged struct into byte array
  uint8_t byteArray[size];
  uint8_t* ptr = (uint8_t*)&val;
  for (uint16_t i = index; i < size; i++) {
    byteArray[i] = *ptr;
    ptr++;
  }

  return queue(command, byteArray, size);
}

size_t CommunicationSerial::read(CommunicationHandlerFunctionPtr func, unsigned long timestamp, unsigned long delta) {
  if (_transfer.available()) {
    CommuicationQueueMessageStruct communication;
    // uint8_t command;
    // uint8_t buffer[BUFFER_MAX_SIZE];
    // size_t size;
    uint16_t recSize = 0;  // bytes we've processed from the receive buffer
    recSize = _transfer.rxObj(communication.command, sizeof(uint8_t));
#ifdef DEBUG
    Serial.print("communication-serial-loop: message command received: ");
    Serial.printf("%d\n", communication.command);
#endif
    recSize = _transfer.rxObj(communication.size, sizeof(size_t));
#ifdef DEBUG
    Serial.print("communication-serial-loop: message bytes to receive: ");
    Serial.printf("%d\n", communication.size);
#endif
    recSize = _transfer.rxObj(communication.buffer, recSize, communication.size);
#ifdef DEBUG
    Serial.print("communication-serial-loop: message bytes received: ");
    for (size_t i = 0; i < communication.size; i++)
        Serial.printf("%d ", communication.buffer[i]);
    Serial.println();
#endif

    if (func != nullptr)
      func(timestamp, delta, communication);

    // TODO: read as many sent messages as are in the buffer using the START_BYTE as demarkation

    return communication.size;
  }

//   if (Serial2.available() > 0) { // Check if data is available
//     //  Serial.println(F("Serial2.available..."));
//     CommuicationQueueMessageStruct communication;

//     communication.size = Serial2.readBytesUntil(';', communication.buffer, BUFFER_MAX_SIZE - 1);
    
// #ifdef DEBUG
//     Serial.println("communication-serial-read: message size received: ");
//     Serial.printf("%d\n", communication.size);
// #endif
//     // recSize = _transfer.rxObj(communication.buffer, recSize, communication.size);

// #ifdef DEBUG
//     Serial.println("communication-serial-read: message bytes to receive: ");
//     for (int i = 0; i < communication.size; i++)
//       Serial.printf("%d ", communication.buffer[i]);
//     Serial.println();
// #endif

  //   if (func != nullptr)
  //       func(timestamp, delta, communication);

  //   return communication.size;
  // }
  return 0;
}

bool CommunicationSerial::setup(unsigned long baud, uint32_t config) {
  Serial2.begin(baud, config);
  // Serial2.begin(1000000);
  while(!Serial2);
  Serial.println("Enabled serial 2...");
  Serial2.setTimeout(10);
#if defined(EPS32)
  Serial2.setRxBufferSize(1024);
#endif

#ifdef DEBUG
  uint8_t debug = 2;
#else
  uint8_t debug = 0;
#endif
  _transfer.begin(Serial2, debug);

#if defined(ESP32)
  mutex = xSemaphoreCreateMutex();
  if (mutex == NULL) {
    Serial.println("Failed to create mutex");
    return false;
  }
#endif

  return true;
}

#if !defined(TEENSYDUINO)
bool CommunicationSerial::setup(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin) {
  Serial2.begin(baud, config, rxPin, txPin);
  while(!Serial2);
  Serial.println("Enabled serial 2...");
  Serial2.setTimeout(10);
#if defined(EPS32)
  Serial2.setRxBufferSize(1024);
#endif

#ifdef DEBUG
  uint8_t debug = 2;
#else
  uint8_t debug = 0;
#endif
  _transfer.begin(Serial2, debug);

#if defined(ESP32)
  mutex = xSemaphoreCreateMutex();
  if (mutex == NULL) {
    Serial.println("Failed to create mutex");
    return false;
  }
#endif

  return true;
}
#endif

CommunicationSerial _communicationSerialObj;