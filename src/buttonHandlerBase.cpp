#include "buttonHandlerBase.h"

void buttonHandlerBase::click(Button2 button) {
  Serial.print(F("buttonHandlerBase.click for '"));
  Serial.print(name());
  Serial.println(F("'!!!!"));
}

void buttonHandlerBase::deinit() {
  // esp_sleep_enable_ext0_wakeup(BUTTON_GPIO, LOW);
}

void buttonHandlerBase::longClick(Button2 button) {
  Serial.print(F("buttonHandlerBase.longClick for '"));
  Serial.print(name());
  Serial.println(F("'!!!!"));
}

void buttonHandlerBase::loop() {
  _btn.loop();
}

char buttonHandlerBase::name() {
  return _name;
}

void buttonHandlerBase::setup(char name, int pin) {
  Serial.print(F("Setup button '"));
  Serial.print(name);
  Serial.println(F("'..."));
  _name = name;

  _btn.begin(pin, INPUT_PULLUP);
  
  _btn.setClickHandler([this](Button2 & b) {
    // Down
    Serial.println(F("Button Down fast")); // It's called upCmd because it increases the index of an array. Visually that would mean the selector goes downwards.
    this->click(b);
  });

  _btn.setLongClickHandler([this](Button2 & b) {
    Serial.println(F("Button Down slow"));
    this->longClick(b);
  });

  Serial.println(F("...button successful."));
  Serial.println();
}