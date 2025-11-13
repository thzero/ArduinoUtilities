#ifndef _FILESYSTEM_BASE_H
#define _FILESYSTEM_BASE_H

#include <FS.h>

class fileSystemBase {
  public:
    virtual File openFile(char* path);
    virtual bool setup();
    virtual long totalBytes();
    virtual long usedBytes();
};

#endif