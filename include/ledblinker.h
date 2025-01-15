#ifndef _DOORBELLE_LEDBLINKER_H_
#define _DOORBELLE_LEDBLINKER_H_

#define DOORBELLE_SET_LED_PATTERN(instance, pattern) \
	instance.setPattern(pattern, sizeof(pattern) / sizeof(pattern[0]))

namespace DoorbellE {
class LedBlinker {
private:
	unsigned char _pin;
	int _onValue;
	const unsigned int* _pattern;
	unsigned int _patternSize;
	bool _isPatternSet;
	unsigned int _currentPosition;
	unsigned long _lastMillis;

public:
	LedBlinker(unsigned char pin, int onValue)
		: _pin(pin)
		, _onValue(onValue) {}
	~LedBlinker() {}

	void start();
	void setPattern(const unsigned int* pattern, unsigned int size);
	void update();
};
}  // namespace DoorbellE

#endif