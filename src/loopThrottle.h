#ifndef _LOOP_THROTTLE_H
#define _LOOP_THROTTLE_H

class loopThrottle {
  public:
    loopThrottle();
    int determine(unsigned long deltaElapsed, int sampleRate);
    void reset();
    
  private:
// #ifdef DEBUG
    // int _countDebug;
// #endif
    int _elapsed;
};

#endif