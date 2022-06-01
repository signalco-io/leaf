#include "SignalcoBootstrapper.h"

SignalcoBootstrapper::SignalcoBootstrapper()
{
    configuration = new SignalcoConfiguration();
    wifi = new SignalcoWifi(configuration);
    server = new SignalcoServer();
    mqtt = new SignalcoMqtt(espClient, configuration);
}

void SignalcoBootstrapper::bootstrapSetup()
{
    Serial.println("Bootstrapper: Initializing...");

    wifi->setupWifi();
    wifi->setupOta();
    server->setup();
    mqtt->setup();

    Serial.println("Bootstrapper: Initialized.");
}

void SignalcoBootstrapper::bootstrapLoop()
{
    wifi->loop();
    mqtt->loop();
    server->loop();
}

void SignalcoBootstrapper::mqttPublish(const char *topic, const char *payload, boolean retained)
{
    mqtt->mqttPublish(topic, payload, retained);
}

void SignalcoBootstrapper::mqttPublish(const char *topic, JsonObject objectToSend, boolean retained)
{
    // Serialize payload
    char buffer[measureJson(objectToSend) + 1];
    serializeJson(objectToSend, buffer, sizeof(buffer));

    mqtt->mqttPublish(topic, buffer, retained);
}

void SignalcoBootstrapper::mqttUnsubscribe(const char *topic)
{
    mqtt->mqttUnsubscribe(topic);
}

void SignalcoBootstrapper::mqttSubscribe(const char *topic)
{
    mqtt->mqttSubscribe(topic);
}

String SignalcoBootstrapper::status()
{
    lastStatus = "MQTT:" + String(mqtt->isOnline()) + " WIFI:" + String(wifi->isConfigured());

    return lastStatus.c_str();
}