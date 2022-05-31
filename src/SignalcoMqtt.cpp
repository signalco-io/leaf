#include "SignalcoMqtt.h"

PubSubClient client(espClient);

SignalcoMqtt::SignalcoMqtt()
{
}

bool SignalcoMqtt::mqttReconnect()
{
    // String mqttClientIdStr = preferences.getString("mqttClientId");
    // const char *mqttClientId = mqttClientIdStr.c_str();
    // if (client.connect(mqttClientId))
    if (client.connect("scounter"))
    {
        // Once connected, publish an announcement...
        auto payload = configuration.getConfigurationString("0");
        this->mqttPublish(discoveryTopic, payload, true);

        // ... and resubscribe
        this->mqttSubscribe(subscribeTopic);
    }
    return client.connected();
}

void SignalcoMqtt::loop()
{
    if (!client.connected())
    {
        long now = millis();
        if (now - this->lastReconnectAttempt > this->mqttReconnectDelayMs)
        {
            this->lastReconnectAttempt = now;

            // Attempt to reconnect
            if (mqttReconnect())
            {
                this->lastReconnectAttempt = 0;
            }
        }
    }
    else
    {
        // Client connected
        client.loop();
    }
}

void SignalcoMqtt::setup()
{
    // if (preferences.isKey("mqttDomain") &&
    //     preferences.isKey("mqttClientId") &&
    //     preferences.isKey("mqttPort"))
    // {
    String mqttDomainStr = preferences.getString("mqttDomain");
    uint16_t mqttPort = preferences.getUShort("mqttPort");

    const char *mqttDomain = mqttDomainStr.c_str();

    client.setBufferSize(1024);
    client.setServer("192.168.0.6", 1883);

    Serial.println("MQTT configured");
    // }
}

void SignalcoMqtt::setCallback(void (*callback)(char *topic, byte *payload, unsigned int length))
{
    client.setCallback(callback);
}

void SignalcoMqtt::mqttPublish(const char *topic, const char *payload, boolean retained)
{
    client.publish(topic, payload, retained);
}

void SignalcoMqtt::mqttUnsubscribe(const char *topic)
{
    client.unsubscribe(topic);
}

void SignalcoMqtt::mqttSubscribe(const char *topic)
{
    client.subscribe(topic);
}

boolean SignalcoMqtt::isOnline()
{
    return client.connected();
}