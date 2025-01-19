#include "doorbell.h"

#include <ArduinoJson.h>
#include <ArduinoLog.h>
#define TONE_USE_INT
#define TONE_PITCH 440
#include <TonePitch.h>

#include "config.h"
#include "utils.h"

namespace DoorbellE {
namespace Doorbell {

const int bellMelody1[] = {NOTE_E4, 4, NOTE_C4, 2};
const int bellMelody2[]
	= {NOTE_E4,
	   2,
	   NOTE_C4,
	   2,
	   NOTE_D4,
	   2,
	   NOTE_G3,
	   1,
	   0,
	   4,
	   NOTE_G3,
	   2,
	   NOTE_D4,
	   2,
	   NOTE_E4,
	   2,
	   NOTE_C4,
	   1};

void Manager::start() {
	_mqttClient.setServer(_mqttConfig.host.c_str(), _mqttConfig.port);

	for (unsigned int i = 0; i < _clientsSize; i++) {
		_clients[i].button->start();
	}

#ifdef BUZZER_PIN
	_melodyPlayer.setMelody(DOORBELLE_ARRAY_AND_SIZE(bellMelody2));
	_melodyPlayer.start();
#endif

	_lastMillis = millis();
}

void Manager::_update(bool isOnline) {
	if (!isOnline) {
		return;
	}

	auto elapsed = millis() - _lastMillis;

	if (!_mqttClient.connected()) {
		_state = State::INITIALIZING;

		if (!_mqttIsConnecting) {
			_mqttIsConnecting = true;

			if (_mqttClient.connect(
					_mqttConfig.clientId.c_str(),
					_mqttConfig.user.c_str(),
					_mqttConfig.password.c_str()
				)) {
				Log.traceln("Connected to MQTT broker");
				_mqttIsConnecting = false;

				for (unsigned int i = 0; i < _clientsSize; i++) {
					auto topicBase = _mqttConfig.internalTopicPrefix + _clients[i].id;
					auto topicOut = topicBase + "/out";
					_mqttClient.subscribe(topicOut.c_str());

					auto topicIn = topicBase + "/in";
					Log.traceln("publish to topic %s", topicIn.c_str());
					_mqttClient.publish(topicIn.c_str(), "tset");
				}
			} else {
				Log.traceln(
					"Failed to connect to MQTT broker, rc=%d",
					_mqttClient.state()
				);
			}
		}

		if (_mqttIsConnecting) {
			if (elapsed >= 1000) {
				_mqttIsConnecting = false;
				_lastMillis = millis();
			}
			return;
		}

		_state = State::IDLE;
	}

	if (_onlineMillis == 0 || millis() - _onlineMillis > 10000) {
		auto topic = _mqttConfig.externalTopicPrefix + "online";
		Log.traceln("publish online message to %s", topic.c_str());
		_mqttClient.publish(topic.c_str(), "");
		_onlineMillis = _onlineMillis == 0 ? 1 : millis();
	}

	_mqttClient.loop();

	if (_state == State::IDLE) {
		for (unsigned int i = 0; i < _clientsSize; i++) {
			if (!_clients[i].button->isPressed()) {
				continue;
			}

			Log.traceln("start calling %s", _clients[i].id.c_str());
			_state = State::CALLING;
			_lastMillis = millis();
			_callAttempts = 0;
			_lastCall = -1;
			_currentClient = i;
			_callId = millis();
#ifdef BUZZER_PIN
			_melodyPlayer.play();
#endif
			break;
		}
	}

	if (_state == State::CALLING) {
		auto elapsed = millis() - _lastMillis;

		if (_callAttempts != _lastCall) {
			_lastCall = _callAttempts;
			Log.traceln("send call %d", _callId);

			JsonDocument msg;
			msg.add(0x5);
			msg.add(_callId);
			char jmsg[255];
			serializeJson(msg, jmsg);

			auto topicIn
				= _mqttConfig.internalTopicPrefix + _clients[_currentClient].id + "/in";
			_mqttClient.publish(topicIn.c_str(), jmsg);
		}

		if (elapsed > 1000) {
			_callAttempts++;
			_lastMillis = millis();
			if (_callAttempts > 3
#ifdef BUZZER_PIN
				&& !_melodyPlayer.isPlaying()
#endif
			) {
				_state = State::IDLE;
				return;
			}
		}
	}

#ifdef BUZZER_PIN
	_melodyPlayer.update();
#endif
}

void Manager::update(bool isOnline) {
	_update(isOnline);

	for (unsigned int i = 0; i < _clientsSize; i++) {
		_clients[i].button->update();
	}
}

}  // namespace Doorbell
}  // namespace DoorbellE
