#include <Arduino.h>
#include <CircularQueue.hpp>
#if defined(ESP32)
#include <mutex>
#endif
#include <SerialTransfer.h>
#if defined(TEENSYDUINO)
#include <TeensyThreads.h>
#endif

#include "communicationSerial.h"
#include <utilities.h>

#define COMMUNICATION_QUEUE_LENGTH 5

#if defined(ESP32)
SemaphoreHandle_t mutex;
std::mutex serial_mtx;
#endif
#if defined(TEENSYDUINO)
Threads::Mutex mutexOutput;
#endif

struct __attribute__((packed)) STRUCT {
  char z;
  float y;
} testStruct;

struct CommuicationQueueMessageStruct {
  uint8_t buffer[1024];
  size_t size;
};

CircularQueue<CommuicationQueueMessageStruct, COMMUNICATION_QUEUE_LENGTH> queueing;

int communicationSerialQueue(uint8_t *byteArray, size_t size) {
#if defined(TEENSYDUINO)
  Threads::Scope m(mutexOutput); // lock on creation
#endif

  Serial.println("communication-serial-queue: trying to queue.");
  Serial.println("communication-serial-queue: requested bytes: ");
  for (size_t i = 0; i < size; i++)
      Serial.printf("%d", byteArray[i]);
  Serial.println();

  if (queueing.size() >= COMMUNICATION_QUEUE_LENGTH) {
    Serial.println("communication-serial-queue: all queues are full.");
    return -1; // exceeded queue length
  }

  CommuicationQueueMessageStruct message;
  memcpy(message.buffer, byteArray, size);
  message.size = size;

  Serial.println("communication-serial-queue: message bytes: ");
  for (size_t i = 0; i < message.size; i++)
      Serial.printf("%d", message.buffer[i]);
  Serial.println();

  queueing.enqueue(message);

  Serial.println("communication-serial-queue: message queued.");
  return 1;
}

int communicationSerialLoop(unsigned long delta) {
#if defined(TEENSYDUINO)
  Threads::Scope m(mutexOutput); // lock on creation
#endif
#if defined(ESP32)
  if (xSemaphoreTake(mutex, portMAX_DELAY)) {
#endif

  if (queueing.isEmpty()) {
    // Serial.println("communication-serial-loop: nothing to send.");
    return 2; // nothing to send.
  }

  CommuicationQueueMessageStruct message = queueing.front();
  queueing.dequeue();
  
  Serial.println("communication-serial-loop: message bytes to send: ");
  for (size_t i = 0; i < message.size; i++)
      Serial.printf("%d", message.buffer[i]);
  Serial.println();

  Serial2.write(message.buffer, message.size);

  memset(message.buffer, 0, 1024);

  Serial.println("communication-serial-loop: sent buffer.");
  return 1;

#if defined(ESP32)
    xSemaphoreGive(mutex); // Release the mutex
}
#endif
}

bool communicationSerialSetup() {
  Serial2.begin(115200);
  // Serial2.begin(1000000);
  while(!Serial2);
  Serial.println("Enabled serial 2...");
  Serial2.setTimeout(10);

#if defined(ESP32)
  mutex = xSemaphoreCreateMutex();
  if (mutex == NULL) {
    Serial.println("Failed to create mutex");
    return false;
  }
#endif

  return true;
}