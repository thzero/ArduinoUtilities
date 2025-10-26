#include <Arduino.h>

#include <utilities.h>

#include "communicationConstants.h"
#include "communicationSerialQueueTransfer.h"

CommunicationSerialQueueTransfer::CommunicationSerialQueueTransfer() {
}

int CommunicationSerialQueueTransfer::_process(CommuicationMessageStruct& message, unsigned long timestamp, unsigned long delta) {
  // uint16_t sendSize = 0;
  // sendSize = _transfer.txObj(message.command, sendSize);
  // sendSize = _transfer.txObj(message.size, sendSize);
  // sendSize = _transfer.txObj(message.buffer, 0, message.size); // Stuff buffer with arra
  uint16_t sendSize = _transfer.txObj(message.buffer, 0, message.size); // Stuff buffer with array
  // Serial.printf(F("communication-serial-transfer-process: message bytes sent: "));
  // Serial.printf(F("%d\n"), sendSize);

  _transfer.sendData(sendSize, message.command);

  return sendSize;
}

bool CommunicationSerialQueueTransfer::_read(CommuicationMessageStruct* communication, unsigned long timestamp, unsigned long delta) {
  if (communication == nullptr)
    return false;
  if (_transfer.available() <= 0)
    return false;

#ifdef DEBUG_COMMUNICATION_SERIAL_READ
  Serial.print("DEBUG_COMMUNICATION_SERIAL_READ: ");
  Serial.println(DEBUG_COMMUNICATION_SERIAL_READ);
#endif

  communication->command = _transfer.currentCommand();
#ifdef DEBUG_COMMUNICATION_SERIAL_READ
  Serial.printf(F("communication-serial-transfer-_read: message command received: %d %d %d\n"), communication->command, _transfer.currentCommand(), (communication->command == _transfer.currentCommand()));
#endif
  communication->size = _transfer.currentReceived();
#ifdef DEBUG_COMMUNICATION_SERIAL_READ
  Serial.printf(F("communication-serial-transfer-_read: message bytes to receive: %d\n"), communication->size);
#endif
//     uint16_t recSize = 0;  // bytes we've processed from the receive buffer
//     recSize = _transfer.rxObj(communication->size, sizeof(size_t));
#ifdef DEBUG_COMMUNICATION_SERIAL_READ
  Serial.printf(F("communication-serial-transfer-_read: message bytes to receive: %d\n"), communication->size);
#endif
  // recSize = _transfer.rxObj(communication->buffer, recSize, communication->size);
  _transfer.rxObj(communication->buffer, 0, communication->size);
  
#ifdef DEBUG_COMMUNICATION_SERIAL_READ
  Serial.printf(F("communication-serial-transfer-_read: message bytes received: %d\n"), communication->size);
  Serial.printf(F("communication-serial-transfer-_read: message bytes received: "));
  for (size_t i = 0; i < communication->size; i++)
      Serial.printf(F("%d "), communication->buffer[i]);
  Serial.println();
  Serial.printf(F("communication-serial-transfer-_read: message command: %d\n"), communication->command);
  Serial.printf(F("communication-serial-transfer-_read: message size: %d\n"), communication->size);
#endif

  return true;
}

int CommunicationSerialQueueTransfer::_send(CommuicationMessageStruct& message) {
  uint16_t sendSize = _transfer.txObj(message.buffer, 0, message.size);
  _transfer.sendData(sendSize, message.command);

  return sendSize;
}

bool CommunicationSerialQueueTransfer::_setup(HardwareSerial& serial, bool debug) {
  CommunicationSerialQueue::_setup(serial, debug);
  _transfer.begin(serial, debug);
  return true;
}
