#include "ledblinker.h"

#include <Arduino.h>
#include <ArduinoLog.h>

namespace DoorbellE {

void LedBlinker::start() { pinMode(_pin, OUTPUT); }

void LedBlinker::update() {
	if (!_isPatternSet || _patternSize == 0) {
		digitalWrite(_pin, _onValue == HIGH ? LOW : HIGH);
		_isPatternSet = false;
		return;
	}

	auto passed = millis() - _lastMillis;

	if (passed >= _pattern[_currentPosition]) {
		_currentPosition++;
		if (_currentPosition >= _patternSize) {
			_currentPosition = 0;
		}
		Log.traceln(
			"change led state to %T after %d passed",
			_currentPosition % 2 == 0,
			passed
		);
		_lastMillis = millis();
	}

	if (_currentPosition % 2 == 0) {
		digitalWrite(_pin, _onValue);
	} else {
		digitalWrite(_pin, _onValue == HIGH ? LOW : HIGH);
	}

	_isPatternSet = false;
}

void LedBlinker::setPattern(const unsigned int* pattern, unsigned int size) {
	if (_isPatternSet) {
		return;
	}

	_isPatternSet = true;
	if (_pattern == pattern && _patternSize == size) {
		return;
	}

	_pattern = pattern;
	_patternSize = size;
	_currentPosition = 0;
	_lastMillis = millis();

	Log.traceln("new led pattern set: %x, %d", pattern, size);
}

}  // namespace DoorbellE