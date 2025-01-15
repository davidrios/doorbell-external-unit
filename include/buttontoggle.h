#ifndef _DOORBELLE_BUTTONTOGGLE_H_
#define _DOORBELLE_BUTTONTOGGLE_H_

namespace DoorbellE {

class ButtonToggle {
private:
	bool _state;
	unsigned char _pin;
	int _pressedOnValue;
	int _lastPressedValue;

public:
	ButtonToggle(unsigned char buttonPin, int pressedOnValue)
		: _pin(buttonPin)
		, _pressedOnValue(pressedOnValue) {}
	~ButtonToggle() {}

	void start();

	void setState(bool state) { _state = state; }
	const bool getState() const { return _state; };

	void update();
};

}  // namespace DoorbellE

#endif