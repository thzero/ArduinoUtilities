#include "buzzer.h"

void buzzer::off() {
  if (!_status)
    return;
    
  noTone(_pin);
  if (_status)
    _status = false;
}

void buzzer::on(int toneValue) {
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