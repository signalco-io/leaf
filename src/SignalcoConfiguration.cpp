#include "SignalcoConfiguration.h"
#include <Preferences.h>

Preferences preferences;

SignalcoConfiguration::SignalcoConfiguration()
{
    preferences.begin("signalco");

    this->wifi = {
        "",
        "",
        "",
        ""};
    this->mqtt = {
        "",
        "",
        123};
}

String combined;

const char *SignalcoConfiguration::getConfigurationString()
{
    return "{\"alias\":\"Prisutnost\",\"mqttTopic\":\"presence\",\"contacts\":[{\"access\":1,\"dataType\":\"double\",\"name\":\"presence\"}]}";
}