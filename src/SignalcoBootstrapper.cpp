#include "SignalcoBootstrapper.h"

void SignalcoBootstrapper::bootstrapSetup()
{
    Serial.println("Bootstrapper: Initializing...");

    preferences.begin("signalco");

    // preferences.putString("hostname", "");
    // preferences.putString("wifissid", "");
    // preferences.putString("wifipassword", "");
    // preferences.putString("otapassword", "");
    // preferences.putString("mqttClientId", "");
    // preferences.putString("mqttDomain", "");
    // preferences.putUShort("mqttPort", 0);

    wifi = new SignalcoWifi();
    if (!wifi->isConfigured())
    {
        Serial.println("Bootstrapper: Configuring WiFi...");

        wifi->setupWifi();
    }

    if (wifi->isConfigured())
    {
        wifi->setupOta();

        server = new SignalcoServer();
        server->setup();

        mqtt = new SignalcoMqtt();
        mqtt->setup();
    }

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