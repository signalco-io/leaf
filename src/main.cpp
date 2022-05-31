#include <Arduino.h>
#include "counter.h"
#include "SignalBootstrapper.h"
#include <WebServer.h>

WebServer server(80);

static SignalBootstrapper *bootstraper;
static SignalcoCounter *counter;

uint8_t lastPresenceCount = 0;
uint8_t loopCounter = 0;

void handleReset()
{
  counter->reset();
  server.sendHeader("Location", "/");
  server.send(307);
}

void handleRoot()
{
  auto presenceString = String(counter->presenceCount);
  auto range1 = String((counter->sensors[0]).range);
  auto range2 = String((counter->sensors[1]).range);
  server.send(200, "text/plain", presenceString + " " + range1 + " " + range2 + bootstraper->status());
}

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

void loop()
{
  if (loopCounter++ % 8 == 0)
  {
    bootstraper->bootstrapLoop();
    server.handleClient();
    loopCounter = 0;
  }

  // Handle counter
  counter->processCounter();
  if (lastPresenceCount != counter->presenceCount)
  {
    lastPresenceCount = counter->presenceCount;
    auto lastCounterStr = String(lastPresenceCount);
    const char *payload = ("{\"contacts\":[{\"contact\":\"presence\",\"value\":" + lastCounterStr + "}]}").c_str();
    bootstraper->mqttPublish("signal/presence", payload);
  }
}
