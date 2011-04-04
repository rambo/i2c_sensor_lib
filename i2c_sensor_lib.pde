// If this is not here we get compile error on i2c_sensor.h (WTF!)
#include <Wire.h>
// If this is not here we get a compile error on i2c_accelerometer.h (WTF!)
#define SimpleFIFO_NONVOLATILE
#include <SimpleFIFO.h> 

#include "dummy_accelerometer.h"

#define DISABLE_PCINT_MULTI_SERVICE
#include <PinChangeInt.h>


bma180 sensor;
int smoothed_data[3];


volatile boolean read_sensor_flag;

int foo;

void read_bm180_data()
{
    //Serial.println("read_bm180_data() called");
    read_sensor_flag = true;
}

void setup()
{
    Serial.begin(115200);  // start serial for output
    Serial.flush();
    
    /*
    Serial.print("I2C_ACCELEROMETER_SMOOTH_BUFFER_SIZE=");
    Serial.println(I2C_ACCELEROMETER_SMOOTH_BUFFER_SIZE, DEC);
    */

    Serial.println("Callign PCintPort::attachInterrupt()");
    pinMode(13, INPUT);
    PCintPort::attachInterrupt(13, &read_bm180_data, RISING);
    Serial.println("PCintPort::attachInterrupt() done");


    Serial.println("Calling sensor.begin()");
    sensor.begin();
    Serial.println("sensor.begin() done");

    sensor.dump_registers(0x09, 0x0e);
    sensor.dump_registers(0x20, 0x25);
    sensor.dump_registers(0x32, 0x37);

    Serial.println("Setting config variables");
    sensor.set_bandwidth(B0000);
    delay(10);
    sensor.set_range(B010);
    delay(10);
    sensor.set_smp_skip(true);
    delay(10);
    sensor.set_new_data_interrupt(true);
    delay(10);
    Serial.println("Setting config variables done");

    sensor.dump_registers(0x09, 0x0e);
    sensor.dump_registers(0x20, 0x25);
    sensor.dump_registers(0x32, 0x37);

    sensor.read_sensor_data();

    
    // Wait a moment so that everything settles.
    delay(10);
    Serial.println("setup() done");
}


void loop()
{
    if (read_sensor_flag)
    {
        sensor.read_sensor_data();
        read_sensor_flag = false;
    }
    foo++;
    if (! (foo % 100))
    {
        sensor.smooth();
        // Array so no need to cast as pointer
        sensor.get_last_smoothed(smoothed_data);
        Serial.print(smoothed_data[0], DEC);
        Serial.print('\t');
        Serial.print(smoothed_data[1], DEC);
        Serial.print('\t');
        Serial.println(smoothed_data[2], DEC);
    }
    delay(1);
}

