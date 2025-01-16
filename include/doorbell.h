#ifndef _DOORBELLE_DOORBELL_H_
#define _DOORBELLE_DOORBELL_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "interruptbutton.h"

namespace DoorbellE {
namespace Doorbell {

struct ClientConfig {
	InterruptButton::Button* button;
	String id;
};

struct MqttConfig {
	String host;
	unsigned int port;
	String clientId;
	String user;
	String password;
	String internalTopicPrefix;
};

enum State {
	INITIALIZING,
	IDLE,
	CALLING,
	CONNECTED,
};

class Manager {
private:
	const MqttConfig& _mqttConfig;
	const ClientConfig* _clients;
	unsigned int _clientsSize;
	State _state;
	WiFiClient _wifiClient;
	PubSubClient _mqttClient;
	bool _mqttIsConnecting;
	unsigned long _lastMillis;

	void _update(bool isOnline);

public:
	Manager(
		const MqttConfig& mqttConfig,
		const ClientConfig* clients,
		unsigned int clientsSize
	)
		: _mqttConfig(mqttConfig)
		, _clients(clients)
		, _clientsSize(clientsSize)
		, _state(State::INITIALIZING)
		, _mqttClient(_wifiClient) {}
	~Manager() {}

	const State state() const { return _state; }

	void start();
	void update(bool isOnline);
};

}  // namespace Doorbell
}  // namespace DoorbellE

#endif