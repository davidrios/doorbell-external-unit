#include <Arduino.h>

#define TONE_USE_INT
#define TONE_PITCH 440
#include <ArduinoLog.h>
#include <TonePitch.h>

#include "buttontoggle.h"
#include "config.h"
#include "doorbell.h"
#include "globals.h"
#include "interruptbutton.h"
#include "ledblinker.h"
#include "melody.h"
#include "network.h"
#include "utils.h"

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
// DoorbellE::ButtonToggle btn1(BTN1_PIN, HIGH);
DoorbellE::MelodyPlayer melodyPlayer(BUZZER_PIN);

const DoorbellE::Doorbell::MqttConfig mqttConfig = {
	MQTT_HOST,
	MQTT_PORT,
	MQTT_CLIENTID,
	MQTT_USER,
	MQTT_PASSWORD,
	INTERNAL_TOPIC_PREFIX,
};

#define DEFINE_CLIENT(pin, mode, id) \
	DOORBELLE_INTERRUPT_BUTTON(__client_##id##_btn, pin, mode)

CLIENTS
#undef DEFINE_CLIENT

#define DEFINE_CLIENT(pin, mode, id) {&__client_##id##_btn, #id},

const DoorbellE::Doorbell::ClientConfig clients[] = {CLIENTS};
#undef DEFINE_CLIENT

DoorbellE::Doorbell::Manager
	doorbellManager(mqttConfig, DOORBELLE_ARRAY_AND_SIZE(clients));

// DOORBELLE_INTERRUPT_BUTTON(btn1, BTN1_PIN, RISING)
// DOORBELLE_INTERRUPT_BUTTON(btn2, BTN2_PIN, FALLING)

void setup() {
	Serial.begin(115200);
	Log.begin(LOG_LEVEL, &Serial, true);

	statusLed.start();
	network.start();
	melodyPlayer.setMelody(DOORBELLE_ARRAY_AND_SIZE(melody1));
	melodyPlayer.start();
	doorbellManager.start();
	// btn1.start();
	// btn2.start();

	Log.traceln("System started");
}

const unsigned int wifiConnectingPattern[] = {50, 200};
const unsigned int mqttConnectingPattern[] = {200, 50};
const unsigned int testPattern[] = {1000, 1000};

void loop() {
	network.update();
	if (!network.isOnline()) {
		statusLed.setPattern(DOORBELLE_ARRAY_AND_SIZE(wifiConnectingPattern));
	}

	doorbellManager.update(network.isOnline());
	if (doorbellManager.state() == DoorbellE::Doorbell::State::INITIALIZING) {
		statusLed.setPattern(DOORBELLE_ARRAY_AND_SIZE(mqttConnectingPattern));
	}

	statusLed.setPattern(NULL, 0);

	// // int btn2State = digitalRead(BTN2_PIN);
	// // Log.traceln("btn2: %d", btn2State);
	// if (btn2.isPressed()) {
	// 	if (!melodyPlayer.isPlaying()) {
	// 		Log.traceln("start melody");
	// 		melodyPlayer.play();
	// 	}
	// }

	// btn1.update();
	// btn2.update();
	melodyPlayer.update();
	statusLed.update();
}
