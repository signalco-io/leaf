#include "SignalcoConfiguration.h"
#include <Arduino.h>

SignalcoConfiguration::SignalcoConfiguration()
{
}

String combined;

const char *SignalcoConfiguration::getConfigurationString(const char *value)
{
    const char *part1 = "{\"alias\":\"Prisutnost\",\"mqttTopic\":\"presence\",\"contacts\":[{\"access\":1,\"dataType\":\"double\",\"name\":\"presence\",\"value\":";
    auto partValue = String(value);
    const char *part2 = "}]}";
    combined = String(part1 + partValue + part2);
    return combined.c_str();
}