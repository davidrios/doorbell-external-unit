#include "interruptbutton.h"

#include <Arduino.h>
#include <ArduinoLog.h>

namespace DoorbellE {
namespace InterruptButton {

void Button::start() {
	pinMode(_pin, INPUT);
	attachInterrupt(_pin, _isr, _mode);
}

void Button::update() { _isrLast = *_isrAction; }

bool Button::isPressed() { return *_isrAction > _isrLast; }

}  // namespace InterruptButton
}  // namespace DoorbellE
