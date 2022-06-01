#ifndef _SIGNALCO_BOOTSTRAPPER_H
#define _SIGNALCO_BOOTSTRAPPER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "SignalcoWifi.h"
#include "SignalcoMqtt.h"
#include "SignalcoServer.h"

class SignalcoWifi;
class SignalcoServer;
class SignalcoMqtt;
class SignalcoConfiguration;

class SignalcoBootstrapper
{
public:
    SignalcoBootstrapper();

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
    SignalcoConfiguration *configuration;

    String lastStatus;
};

#endif