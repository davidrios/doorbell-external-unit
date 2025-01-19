#include "melody.h"

#include <Arduino.h>

namespace DoorbellE {

void MelodyPlayer::start() {
	pinMode(_buzzerPin, OUTPUT);
	digitalWrite(_buzzerPin, HIGH);
}

void MelodyPlayer::setMelody(const int* melody, unsigned int melodySize) {
	_melody = melody;
	_melodySize = melodySize;
}

void MelodyPlayer::update() {
	if (!_isPlaying || _melodySize == 0) {
		digitalWrite(_buzzerPin, HIGH);
		return;
	}

	int divider = 0;
	unsigned long noteDuration = 0;

	divider = _melody[_currentPosition + 1];
	if (divider > 0) {
		// regular note, just proceed
		noteDuration = (wholenote) / divider;
	} else if (divider < 0) {
		// dotted notes are represented with negative durations!!
		noteDuration = (wholenote) / abs(divider);
		noteDuration *= 1.5;  // increases the duration in half for dotted notes
	}

	auto elapsed = millis() - _lastMillis;

	if (elapsed >= noteDuration) {
		_currentPosition += 2;
		if (_currentPosition >= _melodySize) {
			_isPlaying = false;
		}
		_lastMillis = millis();
		_noteStarted = false;
		return;
	}

	auto realDuration = noteDuration * 0.9;

	if (elapsed <= realDuration) {
		if (!_noteStarted) {
			tone(_buzzerPin, _melody[_currentPosition], realDuration);
			_noteStarted = true;
		}
	} else {
		digitalWrite(_buzzerPin, HIGH);
	}
}

void MelodyPlayer::play() {
	_currentPosition = 0;
	_lastMillis = millis();
	_isPlaying = true;
	_noteStarted = false;
}

}  // namespace DoorbellE
