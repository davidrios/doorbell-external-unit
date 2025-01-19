#ifndef _DOORBELLE_DISPLAY_H_
#define _DOORBELLE_DISPLAY_H_

#include <U8x8lib.h>

namespace DoorbellE {
struct DisplayDraw {
	int x = 0;
	int y = 0;
	char* text;
};

class Display {
private:
	const DisplayDraw* _texts;
	unsigned int _textsSize;
	bool _isTextsSet;
	bool _isChanged;
	U8X8_SSD1306_128X32_UNIVISION_SW_I2C _u8x8;

public:
	Display(unsigned char scl, unsigned char sda)
		: _u8x8(scl, sda) {}
	~Display() {}

	void setTexts(const DisplayDraw* texts, unsigned int size);
	void start();
	void update();
};
}  // namespace DoorbellE

#endif