#include <ArduinoJson.h>

#include "commands.h"
#include <debug.h>
#ifdef DEV
#include <i2cscanner.h>
#endif
#include <utilities.h>

deviceCommands::deviceCommands() {
}

void deviceCommands::init(const char* boardNameI, int versionMajorI, int versionMinorI) {
    boardName = boardNameI;
    versionMajor = versionMajorI;
    versionMinor = versionMinorI;
}

// void handlesPreferenceOutput(uint8_t *commandBuffer) {
//   Serial.print(F("$start;\n"));
//   _stateMachine.preferencesOutput();
//   Serial.print(F("$end;\n"));
// }

void deviceCommands::interpretCommandBufferHelp() {
  Serial.println(F(""));
  Serial.print(boardName);
  Serial.println(F(" help..."));

  Serial.print(F("version\t"));
  Serial.print(versionMajor);
  Serial.print(F("."));
  Serial.print(versionMinor);
#if defined(DEV) || defined(DEV_SIM)
  Serial.print(F(" (DEV)"));
#endif
  Serial.println(F(""));

  Serial.println(F("command\tdescription"));
  Serial.println(F("h;\thelp menu"));
#ifdef DEV
  Serial.println(F("i;\tI2C scanner"));
#endif
  Serial.println(F("l;\toutput to serial a list of all flight logs"));
#ifdef DEV
  Serial.println(F("lj;\toutput to serial a list of all flight logs - json"));
#endif

  if (commandsAdditionalHead != NULL) {
    DeviceCommandFunctionEntry *current = commandsAdditionalHead;
    while (current != NULL) {
        Serial.print(current->tag);
        Serial.print(F(";\t"));
        Serial.print(current->description);
        Serial.println("");
        current = current->next;
    }
  }

  Serial.println(F("")); 

#ifdef DEBUG
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F(""));
#endif
}

#if defined(DEV) && defined(I2CSCANNER_ENABLED)
void deviceCommands::interpretCommandBufferI2CScanner() {
  i2CScanner();
}
#endif

DeviceCommandFunctionPtr deviceCommands::getCommandAdditionalFunction(char key) {
  if (commandsAdditionalHead == NULL)
    return NULL;

  // Serial.print("key: ");
  // Serial.println(key);
  DeviceCommandFunctionEntry *current = commandsAdditionalHead;
  while (current != NULL) {
    // Serial.print("key.current: ");
    // Serial.println(current->key);
      if (current->key != key) {
        current = current->next;
        continue;
      }

      return current->func;
  }

  return NULL; // Key not found
}

//  Available commands.
//  The commands can be used via the serial command line or via the Android console

//  h   help
//  i   i2c scanner - DEV only
//
// Other commands are added by the initCommand method
//
void deviceCommands::interpretCommandBuffer() {
  uint8_t command = commandBuffer[0];
  // uint8_t command1 = commandBuffer[1];

#if defined(DWBUG) && defined(DEBUG_COMMAND)
  debug(F("interpretCommandBuffer.command"), command);
  Serial.print(F("interpretCommandBuffer.commandBuffer="));
  for (int i = 0; i < commandbufferLength; i++) {
    Serial.print(commandBuffer[i]);
    // Serial.printf("%c", commandBuffer[i]);
    // Serial.printf("%d", commandBuffer[i]);
  }
  Serial.println(F(""));
#endif  

#if defined(DWBUG) && defined(DEBUG_COMMAND)
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(commandBuffer[0]);
  Serial.println(commandBuffer[1]);
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.println(F(""));
#endif

  // help
  if (command == COMMAND_HELP) {
    interpretCommandBufferHelp();
    return;
  }
#if defined(DEV) && defined(I2CSCANNER_ENABLED)
  // i2c scanner - debug only
  if (command == COMMAND_I2CSCANNER) {
    interpretCommandBufferI2CScanner();
    return;
  }
#endif
#ifdef DEV
  // test command
  if (command == COMMAND_TEST) {
    return;
  }
#endif

  DeviceCommandFunctionPtr commandFunc = getCommandAdditionalFunction(command);
  if (commandFunc != NULL) {
    commandFunc(&commandBuffer[1], commandBufferLength); // Call the function, remove the leading 'command'
    return;
  }

  Serial.print(F("$UNKNOWN command: "));
  // Serial.println(commandBuffer);
  for (int i = 0; i < commandBufferLength; i++)
    Serial.print(commandBuffer[i]);
  Serial.println();
}

void deviceCommands::resetCommandBuffer() {
  memset(commandBuffer, 0, sizeof(commandBuffer));
  commandBufferLength = 0;
}

void deviceCommands::initCommand(char key, DeviceCommandFunctionPtr func, bool visible, const char *tag, const char *description) {
  DeviceCommandFunctionEntry *item = (DeviceCommandFunctionEntry *)malloc(sizeof(DeviceCommandFunctionEntry));
  item->key = key;
  item->func = func;
  item->visible = visible;
  strcpy(item->tag, tag);
  strcpy(item->description, description);
  item->next = NULL;

  if (commandsAdditionalHead == NULL)
    commandsAdditionalHead = item;
  if (commandsAdditionalLatest != NULL)
    commandsAdditionalLatest->next = item;
  commandsAdditionalLatest = item;
}

void deviceCommands::interpretBuffer() {
// #if defined(DWBUG) && defined(DEBUG_COMMAND)
//   Serial.print(F("interpretCommandBuffer.commandBuffer="));
//   for (int i = 0; i < commandBufferLength; i++) {
//   Serial.print(commandBuffer[i]);
//   // Serial.printf("%c", commandBuffer[i]);
//   // Serial.printf("%d", commandBuffer[i]);
//   }
//   Serial.println(F(""));
// #endif

  interpretCommandBuffer();
  resetCommandBuffer();
}

void deviceCommands::interpretBuffer(uint8_t *commandBufferI, int length) {
// #if defined(DWBUG) && defined(DEBUG_COMMAND)
  // Serial.print(F("interpretBuffer.length="));
  // Serial.println(length);
// #endif
  commandBufferLength = length;

// #if defined(DWBUG) && defined(DEBUG_COMMAND)
  // Serial.print(F("interpretBuffer.commandBufferI="));
  // for (int i = 0; i < length; i++) {
  //   Serial.printf("%d", commandBufferI[i]);
  // }
  // Serial.println();
// #endif
  memcpy(commandBuffer, commandBufferI, length);
  
// #if defined(DWBUG) && defined(DEBUG_COMMAND)
  // Serial.print(F("interpretBuffer.commandBuffer="));
  // for (int i = 0; i < length; i++) {
  //   Serial.printf("%d", commandBufferI[i]);
  // }
  // Serial.println();
// #endif

  interpretCommandBuffer();
  resetCommandBuffer();
}

bool deviceCommands::readSerial(unsigned long timestamp, unsigned long delta) {
  uint8_t readVal = ' ';

  // lookup to get data from serial port...
#if defined(DWBUG) && defined(DEBUG_COMMAND)
  debug(F("readSerial.Serial.available"), Serial.available());
#endif
  while (Serial.available()) {
    readVal = Serial.read();
// #if defined(DWBUG) && defined(DEBUG_COMMAND)
#if defined(DWBUG) && defined(DEBUG_COMMAND)
  debug(F("readSerial.readVal"), readVal);
#endif
// #endif

    if (readVal == COMMAND_STOP_BYTE) {
      commandBuffer[commandBufferLength++] = '\0';

// #if defined(DWBUG) && defined(DEBUG_COMMAND)
    // Serial.print(F()"readSerial.commandBuffer.final="));
    // for (int i = 0; i < commandBufferLength; i++)
    //   Serial.print(commandBuffer[i]);
    // Serial.println(F(""));
// #endif
      return true;
    }

    if (readVal != '\n') {
// #if defined(DWBUG) && defined(DEBUG_COMMAND)
    // debug(F("readSerial.commandBufferLength="), commandBufferLength);
// #endif
      if (((unsigned int)commandBufferLength) >= sizeof(commandBuffer)) {
        Serial.println(F("Command buffer overflow, resetting..."));
        resetCommandBuffer();
        return false;
      }

      commandBuffer[commandBufferLength++] = readVal;

// #if defined(DWBUG) && defined(DEBUG_COMMAND)
    // Serial.print(F()"readSerial.commandBuffer="));
    // for (int i = 0; i < commandBufferLength; i++) {
    //   Serial.print(commandBuffer[i]);
    // }
    // Serial.println(F(""));
// #endif
    }
  }

  return false;
}

bool deviceCommands::readSerialInterpret(unsigned long timestamp, unsigned long delta) {
    if (_deviceCommands.readSerial(timestamp, delta)) {
      _deviceCommands.interpretBuffer();
      return true;
    }

    return false;
}

deviceCommands _deviceCommands;