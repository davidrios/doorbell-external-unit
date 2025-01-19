#include <Arduino.h>

#define TONE_USE_INT
#define TONE_PITCH 440
#include <ArduinoLog.h>
#include <TonePitch.h>

#include "config.h"
#ifdef USE_DISPLAY
#include "display.h"
#endif
#include "doorbell.h"
#include "globals.h"
#include "ledblinker.h"
#include "network.h"
#include "utils.h"

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
#ifdef USE_DISPLAY
DoorbellE::Display display(USE_DISPLAY);
#endif

const DoorbellE::Doorbell::MqttConfig mqttConfig
	= {MQTT_HOST,
	   MQTT_PORT,
	   MQTT_CLIENTID,
	   MQTT_USER,
	   MQTT_PASSWORD,
	   INTERNAL_TOPIC_PREFIX,
	   EXTERNAL_TOPIC_PREFIX};

#define DEFINE_CLIENT(pin, mode, id) \
	DOORBELLE_INTERRUPT_BUTTON(__client_##id##_btn, pin, mode)

CLIENTS
#undef DEFINE_CLIENT

#define DEFINE_CLIENT(pin, mode, id) {&__client_##id##_btn, #id},

const DoorbellE::Doorbell::ClientConfig clients[] = {CLIENTS};
#undef DEFINE_CLIENT

DoorbellE::Doorbell::Manager doorbellManager(
	mqttConfig,
	DOORBELLE_ARRAY_AND_SIZE(clients)
#ifdef BUZZER_PIN
		,
	BUZZER_PIN
#endif
);

void setup() {
	Serial.begin(115200);
	Log.begin(LOG_LEVEL, &Serial, true);

	statusLed.start();
	network.start();

#ifdef USE_DISPLAY
	display.start();
#endif

	doorbellManager.start();

	Log.traceln("System started");
}

const unsigned int wifiConnectingPattern[] = {50, 200};
const unsigned int mqttConnectingPattern[] = {200, 50};
const unsigned int callingPattern[] = {1000, 1000};

#ifdef USE_DISPLAY
char connecting[] = "Conectando";
char wifiText[] = "Wi-Fi...";
char mqttText[] = "MQTT...";
char callingText[] = "Chamando";
const DoorbellE::DisplayDraw wifiConnectingDraw[] = {
	{0, 0, connecting},
	{0, 1, wifiText},
};
const DoorbellE::DisplayDraw mqttConnectingDraw[] = {
	{0, 0, connecting},
	{0, 1, mqttText},
};

#define DEFINE_CLIENT(pin, mode, id) \
	char callingClientNameText_##id##_[] = "Casa " #id "...";
CLIENTS
#undef DEFINE_CLIENT

#define DEFINE_CLIENT(pin, mode, id) \
	{{0, 0, callingText}, {0, 1, callingClientNameText_##id##_}},

const DoorbellE::DisplayDraw callingClientsDraw[][2] = {CLIENTS};
#undef DEFINE_CLIENT

#endif

void loop() {
	network.update();
	if (!network.isOnline()) {
		statusLed.setPattern(DOORBELLE_ARRAY_AND_SIZE(wifiConnectingPattern));
#ifdef USE_DISPLAY
		display.setTexts(DOORBELLE_ARRAY_AND_SIZE(wifiConnectingDraw));
#endif
	}

	doorbellManager.update(network.isOnline());
	if (doorbellManager.state() == DoorbellE::Doorbell::State::INITIALIZING) {
		statusLed.setPattern(DOORBELLE_ARRAY_AND_SIZE(mqttConnectingPattern));
#ifdef USE_DISPLAY
		display.setTexts(DOORBELLE_ARRAY_AND_SIZE(mqttConnectingDraw));
#endif
	}

	if (doorbellManager.state() == DoorbellE::Doorbell::State::CALLING) {
		statusLed.setPattern(DOORBELLE_ARRAY_AND_SIZE(callingPattern));

#ifdef USE_DISPLAY
		display.setTexts(DOORBELLE_ARRAY_AND_SIZE(
			callingClientsDraw[doorbellManager.currentClient()]
		));
#endif
	}

#ifdef USE_DISPLAY
	display.setTexts(NULL, 0);
#endif

	statusLed.setPattern(NULL, 0);

#ifdef USE_DISPLAY
	display.update();
#endif

	statusLed.update();
}
