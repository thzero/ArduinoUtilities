#include <Arduino.h>

#include "buzzer.h"

// #define DEBUG
// #define DEBUG_BUZZER

bool buzzer::melody(unsigned long delta) {
  // Serial.println(F("melody._melodyState: %u\n"), _melodyState);
  if (_melodyState == -1) {
    noTone(_pin);
    melodyStop();
    return true;
  }

  // Serial.println(F("melody._melodyIndex: %u\n"), _melodyIndex);
  // Serial.println(F("melody._melodyLength: %u\n"), _melodyLength);
  if (_melodyIndex >= _melodyLength) {
    noTone(_pin);
    melodyStop();
    return true;
  }

  _melodyTime += delta;

  if (_melodyNotePauseTime > 0) {
    if (_melodyTime >= _melodyNotePauseTime * 1000) {
#if defined(DEBUG) && defined(DEBUG_BUZZER)
      Serial.println(F("melody._melodyPause"));
      Serial.printf(F("melody._melodyTime: %u\n"), _melodyTime);
      Serial.printf(F("melody._melodyNotePauseTime: %u\n"), _melodyNotePauseTime);
      Serial.printf(F("melody._melodyNotePauseTime2: %u\n"), _melodyNotePauseTime * 1000);
      Serial.println();
#endif
      noTone(_pin);
      _melodyNotePauseTime = 0;
      _melodyIndex++;
      _melodyTime = 0;
    }
    return false;
  }

#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.println(F("melody.melody..."));
  for (size_t i = 0; i < _melodyLength; i++)
    Serial.printf(F("%d "), _melody[i]);
  Serial.println();

  Serial.println(F("melody.melodyDuration..."));
  for (size_t i = 0; i < _melodyLength; i++)
    Serial.printf(F("%d "), _melodyNoteDurations[i]);
  Serial.println();
#endif

  uint16_t duration = _melodyNoteDurations[_melodyIndex];
  uint16_t toneValue = _melody[_melodyIndex];

#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.printf(F("melody.duration: %d\n"), duration);
  Serial.printf(F("melody.toneValue1: %d\n"), toneValue);
#endif

  tone(_pin, toneValue, duration);

#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.printf(F("melody._meldoyHasNotePauseTimes: %d\n"), _meldoyHasNotePauseTimes);
#endif
  if (!_meldoyHasNotePauseTimes) {
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    _melodyNotePauseTime = duration;
  }
  else
    _melodyNotePauseTime = duration + _melodyNotePauseTimes[_melodyIndex];
#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.printf(F("melody._melodyNotePauseTime: %u\n"), _melodyNotePauseTime);
  Serial.println();
#endif
    
  _melodyTime = 0;
  return false;
}

void buzzer::melodyInit(uint16_t *melody, int16_t *noteDurations, size_t length, uint16_t *notePauseTimes, uint16_t tempo) {
  if (length > BUZZER_BUFFER_LENGTH-1) {
    Serial.printf(F("Song has more than %d notes.\n"), BUZZER_BUFFER_LENGTH);
    return;
  }

  _melodyTempo = tempo;
#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.printf(F("melodyInit._melodyTempo..%d\n"), _melodyTempo);

  Serial.println(F("melodyInit.melody..."));
  for (size_t i = 0; i < length; i++)
    Serial.printf(F("%d "), melody[i]);
  Serial.println();

  Serial.println(F("melodyInit.noteDurations..."));
  for (size_t i = 0; i < length; i++)
    Serial.printf(F("%d "), noteDurations[i]);
  Serial.println();
#endif

  if (_melodyTempo > 0) {
    int wholenote = (60000 * 4) / tempo;
    for (size_t i = 0; i < length; i++) {
      // noteDurations[i] = 1000 / noteDurations[i];
      int duration = noteDurations[i];
      if (duration > 0)
        duration = wholenote / duration;
      else {
        duration = wholenote / abs(duration);
        duration *= 1.5; // increases the duration in half for dotted notes
      }
      noteDurations[i] = duration;
    }

#if defined(DEBUG) && defined(DEBUG_BUZZER)
    Serial.println(F("melodyInit.noteDurations.beat..."));
    for (size_t i = 0; i < length; i++)
      Serial.printf(F("%d "), noteDurations[i]);
    Serial.println();
#endif
  }

#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.println(F("melodyInit.done..."));
  Serial.println();
    
  Serial.printf(F("melodyInit.length..%d\n"), length);
#endif
  // _melody = melody;
  _melodyIndex = 0;
  _melodyLength = length;
  // _melodyNoteDurations = noteDurations;
  // _melodyNotePauseTimes = notePauseTimes;
  _melodyState = -1;
  _melodyTime = 0;

  size_t copyLength = sizeof(uint16_t) * length;

#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.println(F("melodyInit.melody.memcpy.."));
#endif
  memcpy(_melody, melody, copyLength);
#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.println(F("melodyInit._melody..."));
  for (size_t i = 0; i < length; i++)
    Serial.printf(F("%d "), _melody[i]);
  Serial.println();
#endif

#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.println(F("melodyInit.noteDurations.memcpy.."));
#endif
  memcpy(_melodyNoteDurations, noteDurations, copyLength);
#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.println(F("melodyInit._melodyNoteDurations..."));
  for (size_t i = 0; i < length; i++)
    Serial.printf(F("%d "), _melodyNoteDurations[i]);
  Serial.println();
#endif

  if (notePauseTimes != nullptr) {
#if defined(DEBUG) && defined(DEBUG_BUZZER)
    Serial.println(F("melodyInit.notePauseTimes.memcpy.."));
#endif
    memcpy(_melodyNotePauseTimes, notePauseTimes, copyLength);
#if defined(DEBUG) && defined(DEBUG_BUZZER)
    Serial.println(F("melodyInit._melodyNotePauseTimes..."));
    for (size_t i = 0; i < length; i++)
      Serial.printf(F("%d "), _melodyNotePauseTimes[i]);
    Serial.println();
#endif
    _meldoyHasNotePauseTimes = true;
  }

#if defined(DEBUG) && defined(DEBUG_BUZZER)
  Serial.println(F("melodyInit.done..."));
  Serial.println();
#endif
}

void buzzer::melodyReset() {
  // _melody = nullptr;
  _meldoyHasNotePauseTimes = false;
  _melodyIndex = 0;
  _melodyLength = 0;
  // _melodyNoteDurations = nullptr;
  // _melodyNotePauseTimes = nullptr;
  _melodyState = -1;
  _melodyTime = 0;

  memset(_melody, 0, sizeof(uint16_t) * BUZZER_BUFFER_LENGTH);
  memset(_melodyNoteDurations, 0, sizeof(int16_t) * BUZZER_BUFFER_LENGTH);
  memset(_melodyNotePauseTimes, 0, sizeof(uint16_t) * BUZZER_BUFFER_LENGTH);
}

void buzzer::melodyStart() {
  _melodyState = 1;
}

bool buzzer::melodyStatus() {
  return _melodyState > 0;
}

void buzzer::melodyStop() {
  _meldoyHasNotePauseTimes = false;
  _melodyIndex = 0;
  _melodyState = -1;
  _melodyTime = 0;
}

void buzzer::off() {
  if (!_status)
    return;
    
  noTone(_pin);
  _status = false;

  melodyStop();
}

void buzzer::on(int toneValue, int duration = 0) {
  melodyStop();
  if (duration > 0)
    tone(_pin, toneValue, duration);
  else
    tone(_pin, toneValue);
  _status = true;
}

void buzzer::setup(char name, int pin) {
  Serial.print(F("Setup buzzer '"));
  Serial.print(name);
  Serial.println(F("'..."));
  _name = name;
  _pin = pin;

  pinMode(_pin, OUTPUT);

  Serial.println(F("...buzzer successful."));
  Serial.println();
}

bool buzzer::status() {
  return _status;
}
