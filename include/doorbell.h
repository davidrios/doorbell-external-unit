#ifndef _DOORBELLE_DOORBELL_H_
#define _DOORBELLE_DOORBELL_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "interruptbutton.h"
#include "melody.h"

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
	String externalTopicPrefix;
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
	unsigned int _currentClient;
	unsigned int _buzzerPin;
	int _callAttempts;
	int _lastCall;
	unsigned long _callId;
	State _state;
	WiFiClient _wifiClient;
	PubSubClient _mqttClient;
	bool _mqttIsConnecting;
	MelodyPlayer _melodyPlayer;
	unsigned long _lastMillis;
	unsigned long _onlineMillis;

	void _update(bool isOnline);

public:
	Manager(
		const MqttConfig& mqttConfig,
		const ClientConfig* clients,
		unsigned int clientsSize,
		unsigned int buzzerPin = 0
	)
		: _mqttConfig(mqttConfig)
		, _clients(clients)
		, _clientsSize(clientsSize)
		, _buzzerPin(buzzerPin)
		, _state(State::INITIALIZING)
		, _mqttClient(_wifiClient)
		, _melodyPlayer(buzzerPin) {}
	~Manager() {}

	const State state() const { return _state; }

	void start();
	void update(bool isOnline);
	unsigned int currentClient() { return _currentClient; }
};

}  // namespace Doorbell
}  // namespace DoorbellE

#endif