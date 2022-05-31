#ifndef _SIGNALCO_SERVER
#define _SIGNALCO_SERVER

#include <WebServer.h>
#include "SignalcoBootstrapper.h"

class SignalcoServer
{
public:
    SignalcoServer();

    void setup();
    void loop();

private:
    WebServer *server;

    void handleRoot();
};

#endif