#ifndef _DOORBELLE_CONFIG_H_
#define _DOORBELLE_CONFIG_H_

#define LOG_LEVEL LOG_LEVEL_VERBOSE

// D0 is the built-in led
#define STATUS_LED_PIN D0
#define STATUS_LED_ON LOW

// #define BUZZER_PIN D5

// #define USE_DISPLAY D1, D2

#define NETWORK_CHECK_INTERVAL 2000
#define WIFI_SSID "xx"
#define WIFI_KEY "xx"

#define MQTT_HOST "xx"
#define MQTT_PORT 1883
#define MQTT_CLIENTID "doorbell-external"
#define MQTT_USER "doorbell-external"
#define MQTT_PASSWORD "xxx"
#define EXTERNAL_TOPIC_PREFIX "doorbell/external/"
#define INTERNAL_TOPIC_PREFIX "doorbell/internal/"

#define CLIENTS                  \
	DEFINE_CLIENT(D7, RISING, A) \
	DEFINE_CLIENT(D4, RISING, B)

#endif