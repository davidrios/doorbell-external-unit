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

// namespace MelodyN {
// #define TONE_USE_INT
// #define TONE_PITCH 440
// #include <TonePitch.h>

// const int BUZZER_PIN = 0;
// // notes in the melody:
// int melodyo[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

// // note durations: 4 = quarter note, 8 = eighth note, etc.:
// int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

// void playMelody() {
// 	// iterate over the notes of the melody:
// 	for (int thisNote = 0; thisNote < 8; thisNote++) {
// 		// to calculate the note duration, take one second divided by the note type.
// 		// e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
// 		int noteDuration = 1000 / noteDurations[thisNote];
// 		tone(BUZZER_PIN, melodyo[thisNote], noteDuration);

// 		// to distinguish the notes, set a minimum time between them.
// 		// the note's duration + 30% seems to work well:
// 		int pauseBetweenNotes = noteDuration * 1.30;
// 		delay(pauseBetweenNotes);
// 		// stop the tone playing:
// 		noTone(BUZZER_PIN);
// 	}
// }

// void playNokia() {
// 	int melody[] = {
// 		// Nokia Ringtone
// 		// Score available at https://musescore.com/user/29944637/scores/5266155

// 		NOTE_E5,  8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, NOTE_CS5, 8,
// 		NOTE_B4,  8, NOTE_D4, 4, NOTE_E4,  4, NOTE_B4,  8, NOTE_A4,  8,
// 		NOTE_CS4, 4, NOTE_E4, 4, NOTE_A4,  2,
// 	};
// 	int tempo = 180;
// 	int notes = sizeof(melody) / sizeof(melody[0]) / 2;
// 	int wholenote = (60000 * 4) / tempo;
// 	int divider = 0, noteDuration = 0;

// 	// iterate over the notes of the melody.
// 	// Remember, the array is twice the number of notes (notes + durations)
// 	for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
// 		// calculates the duration of each note
// 		divider = melody[thisNote + 1];
// 		if (divider > 0) {
// 			// regular note, just proceed
// 			noteDuration = (wholenote) / divider;
// 		} else if (divider < 0) {
// 			// dotted notes are represented with negative durations!!
// 			noteDuration = (wholenote) / abs(divider);
// 			noteDuration *= 1.5;  // increases the duration in half for dotted notes
// 		}

// 		// we only play the note for 90% of the duration, leaving 10% as a pause
// 		tone(BUZZER_PIN, melody[thisNote], noteDuration * 0.9);

// 		// Wait for the specief duration before playing the next note.
// 		delay(noteDuration);

// 		// stop the waveform generation before the next note.
// 		digitalWrite(BUZZER_PIN, HIGH);
// 	}
// }
// }  // namespace MelodyN

}  // namespace DoorbellE
