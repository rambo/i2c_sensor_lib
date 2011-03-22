// If this is not here we get compile error on i2c_sensor.h
#include <Wire.h>
// If this is not here we get a compile error on i2c_accelerometer.h
#include <SimpleFIFO.h> 

#include "bma180.h"

bma180 sensor;

void setup()
{
    Serial.begin(115200);  // start serial for output
    Serial.flush();
    
    // TODO: initialize the sensor class
    sensor.begin();
    
    // Wait a moment so that everything settles.
    delay(10);
}


void loop()
{
    sensor.read_sensor_data();
    
    // 4Hz main loop
    delay(250);
    Serial.print("mSec since boot: ");
    Serial.println(millis(), DEC);
}

