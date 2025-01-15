#include <Arduino.h>

#define TONE_USE_INT
#define TONE_PITCH 440
#include <ArduinoLog.h>
#include <TonePitch.h>

#include "buttontoggle.h"
#include "config.h"
#include "globals.h"
#include "ledblinker.h"
#include "melody.h"
#include "network.h"

#define BTN1_PIN D1
#define BTN2_PIN D5
#define BUZZER_PIN D2

const int nokiaMelody[] = {
	// Nokia Ringtone
	// Score available at https://musescore.com/user/29944637/scores/5266155

	NOTE_E5, 8,       NOTE_D5,  8,       NOTE_FS4, 4,       NOTE_GS4, 4,       NOTE_CS5,
	8,       NOTE_B4, 8,        NOTE_D4, 4,        NOTE_E4, 4,        NOTE_B4, 8,
	NOTE_A4, 8,       NOTE_CS4, 4,       NOTE_E4,  4,       NOTE_A4,  2,
};

const int melody1[]
	= {NOTE_C4,
	   4,
	   NOTE_G3,
	   8,
	   NOTE_G3,
	   8,
	   NOTE_A3,
	   4,
	   NOTE_G3,
	   4,
	   0,
	   4,
	   NOTE_B3,
	   4,
	   NOTE_C4,
	   4};

DoorbellE::LedBlinker statusLed(STATUS_LED_PIN, STATUS_LED_ON);
DoorbellE::Network network(WIFI_SSID, WIFI_KEY, NETWORK_CHECK_INTERVAL);
DoorbellE::ButtonToggle btn1(BTN1_PIN, HIGH);
DoorbellE::MelodyPlayer melodyPlayer(BUZZER_PIN);

void setup() {
	Serial.begin(115200);
	Log.begin(LOG_LEVEL, &Serial, true);

	statusLed.start();
	network.start();
	btn1.start();
	DOORBELLE_SET_MELODY(melodyPlayer, melody1);
	melodyPlayer.start();

	pinMode(BTN2_PIN, INPUT);

	Log.traceln("System started");
}

const unsigned int wifi_connecting_pattern[] = {50, 200};
const unsigned int test_pattern[] = {1000, 1000};

void loop() {
	network.update();
	if (!network.isOnline()) {
		DOORBELLE_SET_LED_PATTERN(statusLed, wifi_connecting_pattern);
	}
	btn1.update();

	if (btn1.getState()) {
		DOORBELLE_SET_LED_PATTERN(statusLed, test_pattern);
	}

	statusLed.setPattern(NULL, 0);

	int btn2State = digitalRead(BTN2_PIN);
	// Log.traceln("btn2: %d", btn2State);
	if (btn2State == HIGH) {
		if (!melodyPlayer.isPlaying()) {
			Log.traceln("start melody");
			melodyPlayer.play();
		}
	}

	melodyPlayer.update();
	statusLed.update();
}
