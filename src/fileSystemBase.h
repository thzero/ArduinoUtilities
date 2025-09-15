#ifndef _FILESYSTEM_BASE_H
#define _FILESYSTEM_BASE_H

#include <Arduino.h>
#include <FS.h>

class fileSystemBase {
  public:
    virtual File openFile(char* path);
    bool setup();
    long totalBytes();
    long usedBytes();
};

#endif