#include <Arduino.h>
#include "counter.h"
#include "SignalcoBootstrapper.h"

static SignalcoBootstrapper *bootstraper;
static SignalcoCounter *counter;

uint8_t lastPresenceCount = 0;
uint8_t loopCounter = 0;

TaskHandle_t Task1;

void counterTask(void *parameter)
{
  counter->processCounter();
  if (lastPresenceCount != counter->presenceCount)
  {
    lastPresenceCount = counter->presenceCount;
    auto lastCounterStr = String(lastPresenceCount);
    const char *payload = ("{\"contacts\":[{\"contact\":\"presence\",\"value\":" + lastCounterStr + "}]}").c_str();
    bootstraper->mqttPublish("signal/presence", payload);
  }
}

void setup()
{
  Serial.begin(115200);

  bootstraper = new SignalcoBootstrapper();
  bootstraper->bootstrapSetup();

  counter = new SignalcoCounter();
  counter->startCounter();

  xTaskCreatePinnedToCore(
      counterTask,
      "CounterTask",
      10000,
      NULL,
      tskIDLE_PRIORITY,
      &Task1,
      0);
}

void loop()
{
  bootstraper->bootstrapLoop();
}
