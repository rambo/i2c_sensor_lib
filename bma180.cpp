#include "bma180.h"

// Constructor
bma180::bma180() : i2c_accelerometer()
{
}

// Destructor
bma180::~bma180()
{
}

void bma180::begin(byte dev_addr=0x40, boolean wire_begin=true)
{
    int last_data_buffer[5] = {0x8081, 0, 0, 0, 0};
    i2c_accelerometer::begin(dev_addr, wire_begin);
}

void bma180::read_sensor_data()
{
    // Copied from the examples for ADXL345
    Wire.beginTransmission(device_address);
    Wire.send(0x02);
    Wire.endTransmission();
    
    byte read_bytes = 6;
    Wire.requestFrom(device_address, read_bytes);
    byte num = Wire.available(); //device may send less than requested (abnormal)
    byte *buff = (byte*)last_data_buffer+1;
    // Wouldn't this be better with num-- alone on --num ?
    while (num-- > 0)
    {
        *(buff++) = Wire.receive();
    }
    
    // Calculate checksum
    last_data_buffer[4] = last_data_buffer[1] + last_data_buffer[2] + last_data_buffer[3];
    
    // FIXME: Use circular buffer
    byte buffer_i = 0;
    // Put data to buffer
    smooth_buffer[buffer_i][0] = last_data_buffer[1] >> 2;
    smooth_buffer[buffer_i][1] = last_data_buffer[2] >> 2;
    smooth_buffer[buffer_i][2] = last_data_buffer[3] >> 2;
}



