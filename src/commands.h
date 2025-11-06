#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <stdint.h>
#include <map>

// #define DEBUG_COMMAND

typedef void (*DeviceCommandFunctionPtr)(uint8_t* commandBuffer, uint16_t commandBufferLength);

struct DeviceCommandFunctionEntry {
    char key;
    char description[50];
    char descriptionSecondary[50];
    DeviceCommandFunctionPtr func;
    char tag[10];
    char tagSecondary[10];
    bool visible;
    struct DeviceCommandFunctionEntry *next;
};

class deviceCommands {
  public:
    deviceCommands();
    void init(const char* boardNameI, int versionMajorI, int versionMinorI);
    void initCommand(char key, DeviceCommandFunctionPtr func, bool visible, const char *tag, const char *description, const char *tagSecondary = nullptr, const char *descriptionSecondary = nullptr);
    void interpretBuffer();
    void interpretBuffer(uint8_t *commandBufferI, int length);
    bool readSerial(unsigned long timestamp, unsigned long delta);
    bool readSerialInterpret(unsigned long timestamp, unsigned long delta);
  private:
    const char* boardName;
    int versionMajor;
    int versionMinor;
    uint8_t commandBuffer[256];
    int commandBufferLength = 0;
    // DeviceCommandFunctionEntry *commandsAdditionalLatest;
    // DeviceCommandFunctionEntry *commandsAdditionalHead;
    std::map<char, DeviceCommandFunctionEntry*> _commandsAdditional;

    void interpretCommandBufferHelp();
#ifdef DEV
    void interpretCommandBufferI2CScanner();
#endif
    DeviceCommandFunctionPtr getCommandAdditionalFunction(char key);
    void interpretCommandBuffer();
    void resetCommandBuffer();
    void commandsInterpretBuffer();

    const uint8_t COMMAND_HELP  = 'h';
    const uint8_t COMMAND_I2CSCANNER = 'i';
    const uint8_t COMMAND_TEST = 'y';

    const uint8_t COMMAND_STOP_BYTE  = ';';
};

extern deviceCommands _deviceCommands;

#endif