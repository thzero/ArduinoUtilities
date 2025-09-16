#include <debug.h>
#include "loopThrottle.h"

loopThrottle::loopThrottle() {
}

int loopThrottle::determine(unsigned long deltaElapsed, int sampleRate) {
  unsigned long delta = deltaElapsed + _elapsed;
  unsigned long samples = (1000 / sampleRate) * 1000; // convert to micros...
  
// #ifdef DEBUG
  // if (_countDebug > 1000 * 1000) {
  //   debug(F(" "));
  //   debug(F("deltaElapsed"), deltaElapsed);
  //   debug(F("_elapsed"), _elapsed);
  //   debug(F("delta"), delta);
  //   debug(F("sampleRate"), sampleRate);
  //   debug(F("sampleRate / 60"), ((float)sampleRate / 60));
  //   debug(F("samples"), samples);
  //   debug(F("(delta <= samples)"), (delta <= samples));
  //   _countDebug = 0;
  // }
// #endif

  if (delta <= samples) {
// #ifdef DEBUG
    // _countDebug++;
// #endif
    _elapsed = delta;
    return 0;
  }

// #ifdef DEBUG
  // _countDebug = 0;
// #endif
  _elapsed = 0;
  return delta;
};

void loopThrottle::reset() {
// #ifdef DEBUG
  // _countDebug = 0;
// #endif
  _elapsed = 0;
};