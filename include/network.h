#ifndef _DOORBELLE_NETWORK_H_
#define _DOORBELLE_NETWORK_H_

#include <Arduino.h>

namespace DoorbellE {

class Network {
private:
	String _ssid;
	String _passphrase;
	unsigned long _checkInterval;
	bool _isConnecting;
	bool _isOnline;
	unsigned long _lastMillis;

public:
	Network(const String& ssid, const String& passphrase, unsigned long checkInterval)
		: _ssid(ssid)
		, _passphrase(passphrase)
		, _checkInterval(checkInterval) {}
	~Network() {}

	bool isOnline() const { return _isOnline; }

	void start();
	void update();
};

}  // namespace DoorbellE
#endif