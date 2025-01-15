#include "network.h"

#include <ArduinoLog.h>
#include <ESP8266WiFi.h>

namespace DoorbellE {

void Network::start() { _lastMillis = -_checkInterval; }

void Network::update() {
	if (millis() - _lastMillis < _checkInterval) {
		return;
	}

	_lastMillis = millis();

	if (WiFi.status() == WL_CONNECTED) {
		_isOnline = true;

		if (_isConnecting) {
			_isConnecting = false;
			Log.infoln("Wifi connected");
		}
		return;
	}

	_isOnline = false;

	if (!_isConnecting) {
		Log.infoln("Connecting to wifi '%s'", _ssid.c_str());
		WiFi.begin(_ssid, _passphrase);
		_isConnecting = true;
		return;
	}

	Log.warningln("Wifi not connected, status: %d", WiFi.status());
}

}  // namespace DoorbellE