#ifndef _BUTTON_BASE_H
#define _BUTTON_BASE_H

#include <Button2.h>

class buttonHandlerBase {
  public:
    void deinit();
    void loop();
    void setup(char name, int pin);
    virtual void click(Button2 button);
    virtual void doubleClick(Button2 button);
    virtual void longClick(Button2 button);
    virtual void tripleClick(Button2 button);
    char name();
  private:
    Button2 _btn;
    char _name;
};

#endif