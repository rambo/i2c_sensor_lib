#include "i2c_accelerometer.h"

// Constructor
i2c_accelerometer::i2c_accelerometer() : i2c_sensor()
{
}

// Destructor
i2c_accelerometer::~i2c_accelerometer()
{
}

void i2c_accelerometer::begin(byte dev_addr, boolean wire_begin)
{
    i2c_sensor::begin(dev_addr, wire_begin);
}

void i2c_accelerometer::push_to_smoothing_buffer(int val_x, int val_y, int val_z)
{
    smoothing_buffer[0].enqueue(val_x);    
    smoothing_buffer[1].enqueue(val_y);
    smoothing_buffer[2].enqueue(val_z);
}

void i2c_accelerometer::smooth()
{
    Serial.println("DEBUG-BEGIN: i2c_accelerometer::smooth()");
    // Naive smoothing for now
    while (smoothing_buffer[0].count())
    {
        smoothed_buffer[0] = (smoothed_buffer[0] + smoothing_buffer[0].dequeue()) / 2; 
        smoothed_buffer[1] = (smoothed_buffer[0] + smoothing_buffer[1].dequeue()) / 2; 
        smoothed_buffer[2] = (smoothed_buffer[0] + smoothing_buffer[2].dequeue()) / 2; 
    }
    Serial.print(smoothed_buffer[0], DEC);
    Serial.print('\t');
    Serial.print(smoothed_buffer[1], DEC);
    Serial.print('\t');
    Serial.println(smoothed_buffer[2], DEC);
    Serial.println("DEBUG-END: i2c_accelerometer::smooth()");
}

void i2c_accelerometer::get_last_smoothed(int *target)
{
    target[0] = smoothed_buffer[0];
    target[1] = smoothed_buffer[1];
    target[2] = smoothed_buffer[2];
}

