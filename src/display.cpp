#include "display.h"

#include <ArduinoLog.h>

namespace DoorbellE {

void Display::start() {
	_u8x8.begin();
	_u8x8.setFont(u8x8_font_chroma48medium8_r);
}

void Display::update() {
	if (!_isChanged) {
		_isTextsSet = false;
		return;
	}

	if (_texts == NULL) {
		_u8x8.clearDisplay();
	} else {
		for (auto i = 0; i < _textsSize; i++) {
			_u8x8.drawString(_texts[i].x, _texts[i].y, _texts[i].text);
		}
	};

	_isChanged = false;
	_isTextsSet = false;
}

void Display::setTexts(const DisplayDraw* texts, unsigned int size) {
	if (_isTextsSet) {
		return;
	}

	_isTextsSet = true;
	if (_texts == texts && _textsSize == size) {
		return;
	}

	_texts = texts;
	_textsSize = size;
	_isChanged = true;

	Log.traceln("new texts set: %x, %d", texts, size);
}
}  // namespace DoorbellE
