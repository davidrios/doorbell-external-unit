#ifndef _DOORBELLE_MELODY_H_
#define _DOORBELLE_MELODY_H_

#define DOORBELLE_SET_MELODY(instance, melody) \
	instance.setMelody(melody, sizeof(melody) / sizeof(melody[0]))

namespace DoorbellE {

class MelodyPlayer {
private:
	unsigned char _buzzerPin;
	const int* _melody;
	unsigned int _melodySize;
	unsigned int _currentPosition;
	unsigned long _lastMillis;
	bool _isPlaying;
	bool _noteStarted;

	const static int tempo = 180;
	const static int wholenote = (60000 * 4) / tempo;

public:
	MelodyPlayer(unsigned char buzzerPin)
		: _buzzerPin(buzzerPin) {}
	~MelodyPlayer() {}

	void start();
	void setMelody(const int* melody, unsigned int melodySize);
	void update();
	void play();
	void stop() { _isPlaying = false; }
	const bool isPlaying() const { return _isPlaying; }

	template <unsigned int N>
	void set2(int* arr[N]);
};

}  // namespace DoorbellE

#endif