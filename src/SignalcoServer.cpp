#include "SignalcoServer.h"

SignalcoServer::SignalcoServer()
{
    this->server = new WebServer(80);
}

void SignalcoServer::handleRoot()
{
    server->send(200, "text/plain", "Signalco Leaf");
}

void SignalcoServer::setup()
{
    server->on("/",
               [=]()
               { handleRoot(); });
    server->begin();
}

void SignalcoServer::loop()
{
    server->handleClient();
}