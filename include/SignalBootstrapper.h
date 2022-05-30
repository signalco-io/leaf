#ifndef _SIGNAL_BOOTSTRAPPER_H
#define _SIGNAL_BOOTSTRAPPER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "SignalWifi.h"
#include "SignalMqtt.h"

extern Preferences preferences;

class SignalBootstrapper
{
public:
    void bootstrapSetup();
    void bootstrapLoop();
    String status();

    // MQTT
    void mqttPublish(const char *topic, const char *payload, boolean retained = false);
    void mqttPublish(const char *topic, JsonObject objectToSend, boolean retained = false);
    void mqttUnsubscribe(const char *topic);
    void mqttSubscribe(const char *topic);
};

#endif