#include "counter.h"

void SignalcoCounter::initializeSensors()
{
    bool found_any_sensors = false;

    // Set all shutdown pins low to shutdown sensors
    for (int i = 0; i < COUNT_SENSORS; i++)
        digitalWrite(sensors[i].shutdown_pin, LOW);

    delay(10); // give time to shutdown

    for (int i = 0; i < COUNT_SENSORS; i++)
    {
        // one by one enable sensors and set their ID
        digitalWrite(sensors[i].shutdown_pin, HIGH);
        delay(10); // give time to wake up.
        if (sensors[i].psensor->begin(sensors[i].id, false, sensors[i].pwire,
                                      sensors[i].sensor_config))
        {
            found_any_sensors = true;
        }
        else
        {
            Serial.print(i, DEC);
            Serial.print(F(": failed to start\n"));
        }
    }

    if (!found_any_sensors)
    {
        Serial.println("No valid sensors found");
    }
}

void SignalcoCounter::startContinuousRange(uint16_t cycle_time)
{
    if (cycle_time == 0)
        cycle_time = 100;
    Serial.print(F("start Continuous range mode cycle time: "));
    Serial.println(cycle_time, DEC);
    for (uint8_t i = 0; i < COUNT_SENSORS; i++)
    {
        sensors[i].psensor->startRangeContinuous(cycle_time);
    }
    sensors_pending = ALL_SENSORS_PENDING;
    sensor_last_cycle_time = millis();
}

void SignalcoCounter::stopContinuousRange()
{
    Serial.println(F("Stop Continuous range mode"));
    for (uint8_t i = 0; i < COUNT_SENSORS; i++)
    {
        sensors[i].psensor->stopRangeContinuous();
    }
    delay(100); // give time for it to complete.
}

void SignalcoCounter::processContinuousRange()
{
    uint16_t mask = 1;
    for (uint8_t i = 0; i < COUNT_SENSORS; i++)
    {
        if (sensors_pending & mask)
        {
            if (!digitalRead(sensors[i].interrupt_pin)) // TODO: Use interupts
            {
                sensors[i].range = sensors[i].psensor->readRangeResult();
                sensors[i].sensor_status = sensors[i].psensor->readRangeStatus();
                sensors_pending ^= mask;
            }
        }
        mask <<= 1; // setup to test next one
    }

    // See if we have all of our sensors read OK
    uint32_t delta_time = millis() - sensor_last_cycle_time;
    if (!sensors_pending || (delta_time > 1000))
    {
        sensor_last_cycle_time = millis();
        sensors_pending = ALL_SENSORS_PENDING;
    }
}

void SignalcoCounter::startCounter()
{
    this->sensors[0] = {&sensor1, &SENSOR1_WIRE, 0x30, 16, 4,
                        Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_SPEED, 0, 0};
    this->sensors[1] = {&sensor2, &SENSOR2_WIRE, 0x31, 17, 0,
                        Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_SPEED, 0, 0};

    Wire.begin();

    // initialize all of the pins.
    Serial.println(F("VL53LOX_multi start, initialize IO pins"));
    for (int i = 0; i < COUNT_SENSORS; i++)
    {
        pinMode(sensors[i].interrupt_pin, INPUT_PULLUP);
        pinMode(sensors[i].shutdown_pin, OUTPUT);
    }

    initializeSensors();

    startContinuousRange(20);
}

ulong lastPresence = 0;

void SignalcoCounter::processCounter()
{
    processContinuousRange();

    uint16_t value1 = sensors[0].range;
    uint16_t value2 = sensors[1].range;
    if (value1 < 100 || value1 > 1800)
        value1 = UINT16_MAX;
    if (value2 < 100 || value2 > 1800)
        value2 = UINT16_MAX;
    auto sinceLast = millis() - lastPresence;
    if (!isPassing && sinceLast > 500)
    {
        // Trigger direction
        if (value1 != UINT16_MAX || value2 != UINT16_MAX)
        {
            direction = value1 > value2 ? 1 : -1;
        }

        // Wait for both values to trigger
        if (direction != 0 &&
            value1 != UINT16_MAX &&
            value2 != UINT16_MAX)
        {
            isPassing = true;
            this->presenceCount += direction;
            if (this->presenceCount < 0)
                this->presenceCount = 0;
        }
    }
    else if (isPassing)
    {
        if (value1 == UINT16_MAX && value2 == UINT16_MAX)
        {
            isPassing = false;
            direction = 0;
            lastPresence = millis();
        }
    }
}

void SignalcoCounter::reset()
{
    this->presenceCount = 0;
}