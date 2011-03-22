// If this is not here we get compile error on i2c_sensor.h
#include <Wire.h>
// If this is not here we get a compile error on i2c_accelerometer.h
#include <SimpleFIFO.h> 

#include "bma180.h"

#include <PinChangeInt.h>

bma180 sensor;
int smoothed_data[3];

void read_bm180_data()
{
    Serial.println("read_bm180_data called");
    sensor.read_sensor_data();
}

void setup()
{
    Serial.begin(115200);  // start serial for output
    Serial.flush();
    
    // TODO: initialize the sensor class
    
    Serial.println("Calling sensor.begin()");
    sensor.begin();
    sensor.set_bandwidth(600);
    sensor.set_new_data_interrupt(true);
    Serial.println("sensor.begin() done");
    Serial.println("Callign PCintPort::attachInterrupt()");
    PCintPort::attachInterrupt(13, &read_bm180_data, RISING);
    Serial.println("PCintPort::attachInterrupt() done");

    
    // Wait a moment so that everything settles.
    delay(10);
    Serial.println("setup() done");
}


void loop()
{
    sensor.smooth();
    sensor.get_last_smoothed(smoothed_data);
    Serial.print(smoothed_data[0], DEC);
    Serial.print('\t');
    Serial.print(smoothed_data[1], DEC);
    Serial.print('\t');
    Serial.println(smoothed_data[2], DEC);
    delay(50);
}

