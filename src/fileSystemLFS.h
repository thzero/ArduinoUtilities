#ifndef _FILESYSTEM_LFS_H
#define _FILESYSTEM_LFS_H

#include <Arduino.h>
#include <FS.h>

#include "fileSystemBase.h"

class fileSystemLFS: public fileSystemBase {
  public:
    fileSystemLFS();
    File openFile(char* path) override;
    bool setup() override;
    long totalBytes() override;
    long usedBytes() override;
};

#endif