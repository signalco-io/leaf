#include "SignalBootstrapper.h"

Preferences preferences;
SignalWifi signalWifi;
SignalMqtt signalMqtt;

void SignalBootstrapper::bootstrapSetup()
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

    if (!signalWifi.isConfigured())
    {
        Serial.println("Bootstrapper: Configuring WiFi...");

        signalWifi.setupWifi();
    }

    if (signalWifi.isConfigured())
    {
        signalWifi.setupOta();
        signalMqtt.setup();
    }

    Serial.println("Bootstrapper: Initialized.");
}

void SignalBootstrapper::bootstrapLoop()
{
    signalWifi.loop();
    signalMqtt.loop();
}

void SignalBootstrapper::mqttPublish(const char *topic, const char *payload, boolean retained)
{
    signalMqtt.mqttPublish(topic, payload, retained);
}

void SignalBootstrapper::mqttPublish(const char *topic, JsonObject objectToSend, boolean retained)
{
    // Serialize payload
    char buffer[measureJson(objectToSend) + 1];
    serializeJson(objectToSend, buffer, sizeof(buffer));

    signalMqtt.mqttPublish(topic, buffer, retained);
}

void SignalBootstrapper::mqttUnsubscribe(const char *topic)
{
    signalMqtt.mqttUnsubscribe(topic);
}

void SignalBootstrapper::mqttSubscribe(const char *topic)
{
    signalMqtt.mqttSubscribe(topic);
}

String lastStatus;

String SignalBootstrapper::status()
{
    lastStatus = "MQTT:" + String(signalMqtt.isOnline()) + " WIFI:" + String(signalWifi.isConfigured());

    return lastStatus.c_str();
}