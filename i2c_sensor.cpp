#include "i2c_sensor.h"

// Constructor
i2c_sensor::i2c_sensor()
{
}

// Destructor
i2c_sensor::~i2c_sensor()
{
}

void i2c_sensor::begin(byte dev_addr=0x40, boolean wire_begin=true)
{
    device_address = dev_addr;
    if (wire_begin)
    {
        Wire.begin();
    }
}

void i2c_sensor::get_last_raw(byte *target)
{
    // PONDER: Will this actually work for an array or is memcopy magic needed ?
    *target = last_data_buffer;
}


void i2c_sensor::read_sensor_data()
{
}
