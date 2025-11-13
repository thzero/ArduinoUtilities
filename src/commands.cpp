#include <ArduinoJson.h>

#include "commands.h"
#include <debug.h>
#ifdef DEV
#include <i2cscanner.h>
#endif
#include <strings.h>
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

  // if (commandsAdditionalHead != nulptr) {
  //   DeviceCommandFunctionEntry *current = commandsAdditionalHead;
  //   while (current != NULL) {
  //       Serial.print(current->tag);
  //       Serial.print(F(";\t"));
  //       Serial.print(current->description);
  //       Serial.println();
  //       current = current->next;
  //   }
  // }
  for (const auto& pair : _commandsAdditional) {
    Serial.print(pair.second->tag);
    Serial.print(F(";\t"));
    Serial.print(pair.second->description);
    Serial.println();
    
    if (strlen(pair.second->tagSecondary) > 0 && strlen(pair.second->descriptionSecondary) > 0) {
      Serial.print(pair.second->tagSecondary);
      Serial.print(F(";\t"));
      Serial.print(pair.second->descriptionSecondary);
      Serial.println();
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
  // if (commandsAdditionalHead == NULL)
  //   return NULL;

  // Serial.print("key: ");
  // Serial.println(key);
  auto results = _commandsAdditional.find(key);
  if (results != _commandsAdditional.end()) {
#ifdef DEBUG
    Serial.printf("getCommandAdditionalFunction...found '%c'\n", key);
#endif
    DeviceCommandFunctionEntry *current = _commandsAdditional.at(key);
    if (current != nullptr) {
#ifdef DEBUG
      Serial.printf("getCommandAdditionalFunction...did not find '%c'\n", key);
#endif
      return current->func;
    }
  }
#ifdef DEBUG
  else
    Serial.println("getCommandAdditionalFunction...did not find it...");
#endif

  // DeviceCommandFunctionEntry *current = commandsAdditionalHead;
  // while (current != NULL) {
  //   // Serial.print("key.current: ");
  //   // Serial.println(current->key);
  //     if (current->key != key) {
  //       current = current->next;
  //       continue;
  //     }

  //     return current->func;
  // }

  return NULL; // Key not found
}

void deviceCommands::initCommand(char key, DeviceCommandFunctionPtr func, bool visible, const char *tag, const char *description, const char *tagSecondary, const char *descriptionSecondary) {
  if (strlen(tag) > 10) {
    Serial.println("Tag is longer than ten characters.");
    return;
  }
  if (strlen(description) > 50) {
    Serial.println("Description is longer than fifty characters.");
    return;
  }
  if (tagSecondary != nullptr && strlen(tagSecondary) > 10) {
    Serial.println("Tag secondary is longer than ten characters.");
    return;
  }
  if (descriptionSecondary != nullptr && strlen(descriptionSecondary) > 50) {
    Serial.println("Description secondary is longer than fifty characters.");
    return;
  }

  DeviceCommandFunctionEntry *item = (DeviceCommandFunctionEntry *)malloc(sizeof(DeviceCommandFunctionEntry));
  item->key = key;
  item->func = func;
  item->visible = visible;
  strcpy(item->tag, tag);
  if (tagSecondary != nullptr)
    strcpy(item->tagSecondary, tagSecondary);
  strcpy(item->description, description);
  if (descriptionSecondary != nullptr)
    strcpy(item->descriptionSecondary, descriptionSecondary);
  item->next = NULL;

  // if (commandsAdditionalHead == NULL)
  //   commandsAdditionalHead = item;
  // if (commandsAdditionalLatest != NULL)
  //   commandsAdditionalLatest->next = item;
  // commandsAdditionalLatest = item;

  _commandsAdditional.insert(std::make_pair(key, item));
}

void deviceCommands::interpretBuffer() {
#ifdef DEBUG_COMMAND
  Serial.print(F("interpretBuffer.commandBuffer="));
  for (int i = 0; i < commandBufferLength; i++) {
    Serial.print(commandBuffer[i]);
    // Serial.printf("%c", commandBuffer[i]);
    // Serial.printf("%d", commandBuffer[i]);
  }
  Serial.println(F(""));
#endif

  interpretCommandBuffer();
  resetCommandBuffer();
}

void deviceCommands::interpretBuffer(uint8_t *commandBufferI, int length) {
#ifdef DEBUG_COMMAND
  Serial.print(F("interpretBuffer2.length="));
  Serial.println(length);
#endif
  commandBufferLength = length;

#ifdef DEBUG_COMMAND
  Serial.print(F("interpretBuffer2.commandBufferI="));
  for (int i = 0; i < length; i++) {
    Serial.printf("%d", commandBufferI[i]);
  }
  Serial.println();
#endif
  memcpy(commandBuffer, commandBufferI, length);
  
#ifdef DEBUG_COMMAND
  Serial.print(F("interpretBuffer2.commandBuffer="));
  for (int i = 0; i < length; i++) {
    Serial.printf("%d", commandBufferI[i]);
  }
  Serial.println();
#endif

  interpretCommandBuffer();
  resetCommandBuffer();
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

#ifdef DEBUG_COMMAND
  debug(F("interpretCommandBuffer.command"), command);
  Serial.print(F("interpretCommandBuffer.commandBuffer="));
  for (int i = 0; i < commandBufferLength; i++) {
    Serial.print(commandBuffer[i]);
    // Serial.printf("%c", commandBuffer[i]);
    // Serial.printf("%d", commandBuffer[i]);
  }
  Serial.println(F(""));
#endif  

#ifdef DEBUG_COMMAND
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

bool deviceCommands::readSerial(unsigned long timestamp, unsigned long delta) {
  uint8_t readVal = ' ';

  // lookup to get data from serial port...
// #ifdef DEBUG_COMMAND
//   debug(F("readSerial.Serial.available"), Serial.available());
// #endif
  while (Serial.available()) {
    readVal = Serial.read();
// #ifdef DEBUG_COMMAND
//   debug(F("readSerial.readVal"), readVal);
// #endif

    if (readVal == COMMAND_STOP_BYTE) {
      commandBuffer[commandBufferLength++] = '\0';

#ifdef DEBUG_COMMAND
    Serial.print(F("readSerial.commandBuffer.final="));
    for (int i = 0; i < commandBufferLength; i++)
      Serial.print(commandBuffer[i]);
    Serial.println(F(""));
#endif
      return true;
    }

    if (readVal != '\n') {
#ifdef DEBUG_COMMAND
    debug(F("readSerial.commandBufferLength="), commandBufferLength);
#endif
      if (((unsigned int)commandBufferLength) >= sizeof(commandBuffer)) {
        Serial.println(F("Command buffer overflow, resetting..."));
        resetCommandBuffer();
        return false;
      }

      commandBuffer[commandBufferLength++] = readVal;

#ifdef DEBUG_COMMAND
    Serial.print(F("readSerial.commandBuffer="));
    for (int i = 0; i < commandBufferLength; i++) {
      Serial.print(commandBuffer[i]);
    }
    Serial.println(F(""));
#endif
    }
  }

  return false;
}

bool deviceCommands::readSerialInterpret(unsigned long timestamp, unsigned long delta) {
    if (readSerial(timestamp, delta)) {
#ifdef DEBUG_COMMAND
    Serial.print(F("readSerial.commandBuffer="));
    for (int i = 0; i < commandBufferLength; i++) {
      Serial.print(commandBuffer[i]);
    }
    Serial.println(F(""));
#endif
      interpretBuffer();
      return true;
    }

    return false;
}

void deviceCommands::resetCommandBuffer() {
  memset(commandBuffer, 0, sizeof(commandBuffer));
  commandBufferLength = 0;
}

deviceCommands _deviceCommands;