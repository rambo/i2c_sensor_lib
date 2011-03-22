#include "i2c_sensor.h"

// Constructor
i2c_sensor::i2c_sensor()
{
}

// Destructor
i2c_sensor::~i2c_sensor()
{
}

void i2c_sensor::begin(byte dev_addr, boolean wire_begin)
{
    device_address = dev_addr;
    if (wire_begin)
    {
        Wire.begin();
    }
}

void i2c_sensor::get_last_raw(void *target)
{
    memcpy(&target, &last_data_buffer, sizeof(&target));
}


void i2c_sensor::get_last_smoothed(void *target)
{
    // This seems to give all zeros
    memcpy(&target, &smoothed_buffer, sizeof(&target));
}

void i2c_sensor::read_sensor_data()
{
}
