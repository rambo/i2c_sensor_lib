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
    
}
