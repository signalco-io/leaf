#include <Arduino.h>
#include "counter.h"
#include "SignalBootstrapper.h"
#include <WebServer.h>

WebServer server(80);

static SignalBootstrapper *bootstraper;
static SignalcoCounter *counter;

void handleReset()
{
  counter->reset();
}

void handleRoot()
{
  auto presenceString = String(counter->presence);
  auto range1 = String((counter->sensors[0]).range);
  auto range2 = String((counter->sensors[1]).range);
  server.send(200, "text/plain", presenceString + " " + range1 + " " + range2 + bootstraper->status());
}

//====================================================================
// Setup
//====================================================================
void setup()
{
  Serial.begin(115200);

  bootstraper = new SignalBootstrapper();
  bootstraper->bootstrapSetup();

  server.on("/", handleRoot);
  server.on("/reset", handleReset);
  server.begin();

  counter = new SignalcoCounter();
  counter->startCounter();
}

//====================================================================
// loop
//====================================================================
int lastCounter = 0;
int loopCounter = 0;

void loop()
{
  if (loopCounter++ % 8 == 0)
  {
    bootstraper->bootstrapLoop();
    server.handleClient();
    loopCounter = 0;
  }
  counter->processCounter();

  if (lastCounter != counter->presence)
  {
    lastCounter = counter->presence;
    auto lastCounterStr = String(lastCounter);
    const char *payload = ("{\"contacts\":[{\"contact\":\"presence\",\"value\":" + lastCounterStr + "}]}").c_str();
    bootstraper->mqttPublish("signal/presence", payload);
  }
}
