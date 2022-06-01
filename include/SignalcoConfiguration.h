#ifndef _SIGNALCO_CONFIGURATION_H
#define _SIGNALCO_CONFIGURATION_H

#include <Arduino.h>

struct WifiConfiguration
{
    const char *hostname;
    const char *ssid;
    const char *password;
    const char *otaPassword;
};

struct MqttConfiguration
{
    const char *domain;
    const char *clientId;
    uint16_t port;
};

class SignalcoConfiguration
{
public:
    WifiConfiguration wifi;
    MqttConfiguration mqtt;

    SignalcoConfiguration();
    const char *getConfigurationString();
};

#endif