#ifndef _SIGNALCO_COUNTER_H
#define _SIGNALCO_COUNTER_H

#include <Adafruit_VL53L0X.h>
#include <Wire.h>

// Define which Wire objects to use, may depend on platform
// or on your configurations.
#define SENSOR1_WIRE Wire
#define SENSOR2_WIRE Wire

typedef struct
{
    Adafruit_VL53L0X *psensor; // pointer to object
    TwoWire *pwire;
    int id;            // id for the sensor
    int shutdown_pin;  // which pin for shutdown;
    int interrupt_pin; // which pin to use for interrupts.
    Adafruit_VL53L0X::VL53L0X_Sense_config_t
        sensor_config;     // options for how to use the sensor
    uint16_t range;        // range value used in continuous mode stuff.
    uint8_t sensor_status; // status from last ranging in continuous.
} sensorList_t;

class SignalcoCounter
{
private:
    Adafruit_VL53L0X sensor1;
    Adafruit_VL53L0X sensor2;

    const int COUNT_SENSORS = sizeof(sensors) / sizeof(sensors[0]);

    const uint16_t ALL_SENSORS_PENDING = ((1 << COUNT_SENSORS) - 1);
    uint16_t sensors_pending = ALL_SENSORS_PENDING;
    uint32_t sensor_last_cycle_time;

    bool isPassing = false;
    int8_t direction = 0;

    void initializeSensors();
    void startContinuousRange(uint16_t cycle_time);
    void stopContinuousRange();
    void processContinuousRange();

public:
    uint8_t presenceCount = 0;
    sensorList_t sensors[2];
    void startCounter();
    void processCounter();
    void reset();
};

#endif
