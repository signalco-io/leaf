#ifndef _SIGNALCO_WIFI_H
#define _SIGNALCO_WIFI_H

#include <WiFi.h>
#include <ArduinoOTA.h>
#include "SignalcoBootstrapper.h"
#include "SignalcoConfiguration.h"

extern WiFiClient espClient;

class SignalcoWifi
{
public:
    SignalcoWifi(SignalcoConfiguration *configuration);

    void setupWifi();
    void setupOta();
    void loop();

    bool isConfigured();

private:
    SignalcoConfiguration *configuration;
};

#endif