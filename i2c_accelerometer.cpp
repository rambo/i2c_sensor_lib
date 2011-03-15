#include "i2c_accelerometer.h"

// Constructor
i2c_accelerometer::i2c_accelerometer() : i2c_sensor()
{
}

// Destructor
i2c_accelerometer::~i2c_accelerometer()
{
}

void i2c_accelerometer::begin(byte dev_addr=0x40, boolean wire_begin=true)
{
    i2c_sensor::begin(dev_addr, wire_begin);
}

