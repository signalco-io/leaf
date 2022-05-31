#ifndef _SIGNALCO_MQTT_H
#define _SIGNALCO_MQTT_H

#include <PubSubClient.h>
#include "SignalcoWifi.h"
#include "SignalcoConfiguration.h"
#include "SignalcoBootstrapper.h"

class SignalcoMqtt
{
private:
    const char *subscribeTopic = "signal/";
    const char *discoveryTopic = "signal/discovery/presence/config";
    long lastReconnectAttempt;
    const int mqttReconnectDelayMs = 5000;
    SignalcoConfiguration configuration;

    bool mqttReconnect();

public:
    SignalcoMqtt();

    void setup();
    void loop();

    void setCallback(void (*callback)(char *topic, byte *payload, unsigned int length));
    void mqttPublish(const char *topic, const char *payload, boolean retained = false);
    void mqttUnsubscribe(const char *topic);
    void mqttSubscribe(const char *topic);
    boolean isOnline();
};

#endif