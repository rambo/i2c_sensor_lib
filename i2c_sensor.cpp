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

void i2c_sensor::readFrom(byte address, byte num, byte *buff) {
  Wire.beginTransmission(device_address); //start transmission to device 
  Wire.send(address);        //sends address to read from
  Wire.endTransmission(); //end transmission

  Wire.requestFrom(device_address, num);    // request num bytes from device
  num = Wire.available(); //device may send less than requested (abnormal)
  while(num-- > 0) {
    *(buff++) = Wire.receive(); // receive a byte
  }
}

