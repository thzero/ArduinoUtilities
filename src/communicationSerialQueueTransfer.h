#ifndef _COMMMUNICATION_SERIAL_QUEUE_TRANSFER_H
#define _COMMMUNICATION_SERIAL_QUEUE_TRANSFER_H

#include <SerialTransfer.h>

#include "communicationData.h"
#include "communicationSerialQueue.h"

class CommunicationSerialQueueTransfer: public CommunicationSerialQueue {
  public:
    CommunicationSerialQueueTransfer();
    
  protected:
    int _process(CommuicationMessageStruct& message, unsigned long timestamp, unsigned long delta) override;
    bool _setup(HardwareSerial& serial, bool debug) override;
    bool _read(CommuicationMessageStruct* communication, unsigned long timestamp, unsigned long delta) override;
    int _send(CommuicationMessageStruct& message) override;
    
  private:
    SerialTransfer _transfer;
};

#endif