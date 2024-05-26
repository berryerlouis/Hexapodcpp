#include "Led.h"

Led::Led(const uint8_t gpio)
  : mIo(gpio),
    mState(OFF) {
  pinMode(this->mIo, OUTPUT);
  Off();
}


bool Led::On() {
  digitalWrite(this->mIo, LOW);
  this->mState = ON;
  return true;
}

bool Led::Off() {
  digitalWrite(this->mIo, HIGH);
  this->mState = OFF;
  return true;
}

bool Led::Toggle() {
  if (ON == this->Get()) {
    this->Off();
  } else {
    this->On();
  }
  return true;
}

Led::LedState Led::Get() const {
  return this->mState;
}
