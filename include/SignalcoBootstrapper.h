#ifndef _SIGNALCO_BOOTSTRAPPER_H
#define _SIGNALCO_BOOTSTRAPPER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "SignalcoWifi.h"
#include "SignalcoMqtt.h"
#include "SignalcoServer.h"

extern Preferences preferences;

class SignalcoWifi;
class SignalcoServer;
class SignalcoMqtt;

class SignalcoBootstrapper
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

private:
    SignalcoWifi *wifi;
    SignalcoMqtt *mqtt;
    SignalcoServer *server;

    Preferences preferences;
    String lastStatus;
};

#endif