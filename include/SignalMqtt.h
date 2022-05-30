#ifndef _SIGNAL_MQTT_H
#define _SIGNAL_MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include "SignalBootstrapper.h"
#include "SignalWifi.h"
#include "SignalConfiguration.h"

class SignalMqtt
{
private:
    const char *subscribeTopic = "signal/";
    const char *discoveryTopic = "signal/discovery/presence/config";
    long lastReconnectAttempt;
    const int mqttReconnectDelayMs = 5000;
    SignalConfiguration configuration;

    bool mqttReconnect();

public:
    void setup();
    void loop();

    void setCallback(void (*callback)(char *topic, byte *payload, unsigned int length));
    void mqttPublish(const char *topic, const char *payload, boolean retained = false);
    void mqttUnsubscribe(const char *topic);
    void mqttSubscribe(const char *topic);
    boolean isOnline();
};

#endif