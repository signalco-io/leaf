#ifndef _SIGNALCO_CONFIGURATION_H
#define _SIGNALCO_CONFIGURATION_H

class SignalcoConfiguration
{
public:
    SignalcoConfiguration();

    const char *getConfigurationString(const char *value);
};

#endif