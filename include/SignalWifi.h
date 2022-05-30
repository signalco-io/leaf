#ifndef _SIGNAL_WIFI_H
#define _SIGNAL_WIFI_H

#include <WiFi.h>
#include <ArduinoOTA.h>
#include "SignalBootstrapper.h"

extern WiFiClient espClient;

class SignalWifi
{
public:
    void setupWifi();
    void setupOta();
    void loop();

    bool isConfigured();
};

#endif