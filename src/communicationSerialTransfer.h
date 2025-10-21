#ifndef _COMMMUNICATION_QUEUED_SERIAL_TRANSFER_H
#define _COMMMUNICATION_QUEUED_SERIAL_TRANSFER_H

#include <SerialTransfer.h>

#include "communicationData.h"
#include "communicationSerial.h"

class CommunicationSerialTransfer: public CommunicationSerial {
  public:
    CommunicationSerialTransfer();
    
  protected:
    int _process(CommuicationQueueMessageStruct& message, unsigned long timestamp, unsigned long delta) override;
    bool _setup(HardwareSerial& serial, bool debug) override;
    bool _read(CommuicationQueueMessageStruct* communication, unsigned long timestamp, unsigned long delta) override;
    int _send(CommuicationQueueMessageStruct& message) override;
    
  private:
    SerialTransfer _transfer;
};

#endif