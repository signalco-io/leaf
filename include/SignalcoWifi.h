#ifndef _SIGNALCO_WIFI_H
#define _SIGNALCO_WIFI_H

#include <WiFi.h>
#include <ArduinoOTA.h>
#include "SignalcoBootstrapper.h"

extern WiFiClient espClient;

class SignalcoWifi
{
public:
    SignalcoWifi();

    void setupWifi();
    void setupOta();
    void loop();

    bool isConfigured();
};

#endif