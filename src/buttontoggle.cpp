#include "buttontoggle.h"

#include <ArduinoLog.h>

namespace DoorbellE {
void ButtonToggle::start() {
	_state = false;
	_lastPressedValue = _pressedOnValue == HIGH ? LOW : HIGH;
	pinMode(_pin, INPUT);
}

void ButtonToggle::update() {
	int state = digitalRead(_pin);

	if (state == _pressedOnValue && _lastPressedValue != _pressedOnValue) {
		_state = !_state;
		Log.traceln("button toggled: %T", _state);
	}

	_lastPressedValue = state;
}
}  // namespace DoorbellE