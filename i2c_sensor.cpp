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

// TODO:Doublecheck my pointer logic
void i2c_sensor::get_last_raw(void *target)
{
    memcpy(&target, &last_data_buffer, sizeof(&target));
}
void i2c_sensor::get_last_smoothed(void *target)
{
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

boolean i2c_sensor::read(byte address, byte *target)
{
    //return i2c_sensor::read_many(address, 1, target);
    Wire.beginTransmission(device_address);
    Wire.send(address);
    byte result = Wire.endTransmission();
    if (result > 0)
    {
        Serial.print("Read failed, Wire.endTransmission returned: ");
        Serial.println(result, DEC);
        return false;
    }
    byte req_num = 1;
    Wire.requestFrom(device_address, req_num);
    byte recv_num =  Wire.available();
    if (recv_num != req_num)
    {
        // Unexpected amount of data to be received, clear the buffers and return failure
        while (recv_num-- > 0)
        {
              Wire.receive();
        }
        Serial.println("Read failed, unexpected amount of data");
        return false;
    }
    // TODO: Doublecheck the pointer logic here
    *target = Wire.receive();
    return true;
}

boolean i2c_sensor::read_many(byte address, byte req_num, byte *target)
{
    Wire.beginTransmission(device_address);
    Wire.send(address);
    byte result = Wire.endTransmission();
    if (result > 0)
    {
        return false;
    }
    Wire.requestFrom(device_address, req_num);
    byte recv_num =  Wire.available();
    if (recv_num != req_num)
    {
        // Unexpected amount of data to be received, clear the buffers and return failure
        while (recv_num-- > 0)
        {
              Wire.receive();
        }
        return false;
    }
    while(recv_num-- > 0)
    {
        // First assign the return of Wire.receive to where the pointer points to, then incement the pointer (so in next iteration we write to correct place)
        *(target++) = Wire.receive();
    }
}


boolean i2c_sensor::write(byte address, byte value)
{
    Wire.beginTransmission(device_address);
    Wire.send(address);
    Wire.send(value);
    byte result = Wire.endTransmission();
    if (result > 0)
    {
        return false;
    }
    return true;
}

boolean i2c_sensor::write_many(byte address, byte num, byte *source)
{
    Wire.beginTransmission(device_address);
    Wire.send(address);
    Wire.send(source, num);
    byte result = Wire.endTransmission();
    if (result > 0)
    {
        return false;
    }
    return true;
}



