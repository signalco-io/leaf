#ifndef _SIGNALCO_MQTT_H
#define _SIGNALCO_MQTT_H

#include <PubSubClient.h>
#include "SignalcoBootstrapper.h"
#include "SignalcoWifi.h"
#include "SignalcoConfiguration.h"

class SignalcoMqtt
{
private:
    const char *subscribeTopic = "signal/";                          // TODO: Move to config
    const char *discoveryTopic = "signal/discovery/presence/config"; // TODO: Move to config
    long lastReconnectAttempt;
    const int mqttReconnectDelayMs = 15000;
    SignalcoConfiguration *configuration;
    PubSubClient *client;

    bool mqttReconnect();

public:
    SignalcoMqtt(WiFiClient &wifiClient, SignalcoConfiguration *configuration);

    void setup();
    void loop();

    void setCallback(void (*callback)(char *topic, byte *payload, unsigned int length));
    void mqttPublish(const char *topic, const char *payload, boolean retained = false);
    void mqttUnsubscribe(const char *topic);
    void mqttSubscribe(const char *topic);
    boolean isOnline();
};

#endif