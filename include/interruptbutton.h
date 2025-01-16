#ifndef _DOORBELLE_INTERRUPTBUTTON_H_
#define _DOORBELLE_INTERRUPTBUTTON_H_

#include <Arduino.h>

#define DOORBELLE_INTERRUPT_BUTTON(name, pin, mode)                 \
	volatile DoorbellE::InterruptButton::ISRInfo __##name##_action; \
	void IRAM_ATTR __##name##_isr() {                               \
		if (millis() - __##name##_action.lastTime > 250) {          \
			__##name##_action.pressed = false;                      \
		}                                                           \
                                                                    \
		if (!__##name##_action.pressed) {                           \
			__##name##_action.incr++;                               \
			__##name##_action.pressed = true;                       \
			__##name##_action.lastTime = millis();                  \
		}                                                           \
	}                                                               \
	DoorbellE::InterruptButton::Button                              \
		name(pin, mode, __##name##_isr, &__##name##_action.incr);

namespace DoorbellE {
namespace InterruptButton {

struct ISRInfo {
	int incr = 0;
	unsigned long lastTime;
	bool pressed;
};

class Button {
private:
	unsigned char _pin;
	int _mode;
	void (*_isr)();
	volatile int* _isrAction;
	int _isrLast = 0;

public:
	Button(
		unsigned char buttonPin,
		int buttonMode,
		void (*isr)(),
		volatile int* isrAction
	)
		: _pin(buttonPin)
		, _mode(buttonMode)
		, _isr(isr)
		, _isrAction(isrAction) {}
	~Button() {}

	void start();
	void update();
	bool isPressed();
};

}  // namespace InterruptButton
}  // namespace DoorbellE

#endif