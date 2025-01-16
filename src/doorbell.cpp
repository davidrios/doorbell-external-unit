#include "doorbell.h"

#include <ArduinoLog.h>

namespace DoorbellE {
namespace Doorbell {

void Manager::start() {
	_mqttClient.setServer(_mqttConfig.host.c_str(), _mqttConfig.port);

	for (unsigned int i = 0; i < _clientsSize; i++) {
		_clients[i].button->start();
	}

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
	}

	_mqttClient.loop();
	_state = State::IDLE;

	for (unsigned int i = 0; i < _clientsSize; i++) {
		if (_clients[i].button->isPressed()) {
			Log.traceln("pressed button for client %s", _clients[i].id.c_str());
		}
	}
}

void Manager::update(bool isOnline) {
	_update(isOnline);

	for (unsigned int i = 0; i < _clientsSize; i++) {
		_clients[i].button->update();
	}
}

}  // namespace Doorbell
}  // namespace DoorbellE
