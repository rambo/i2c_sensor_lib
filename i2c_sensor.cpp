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
//    memcpy(target, &last_data_buffer, sizeof(*target));
}
void i2c_sensor::get_last_smoothed(void *target)
{
//    memcpy(target, &smoothed_buffer, sizeof(*target));
}

void i2c_sensor::read_sensor_data()
{
}


boolean i2c_sensor::read(byte address, byte *target)
{
    return i2c_sensor::read_many(address, 1, target);
}
byte i2c_sensor::read(byte address)
{
    byte target;
    i2c_sensor::read_many(address, 1, &target);
    return target;
}



boolean i2c_sensor::read_many(byte address, byte req_num, byte *target)
{
    Wire.beginTransmission(device_address);
    Wire.send(address);
    byte result = Wire.endTransmission();
    if (result > 0)
    {
        Serial.print("DEBUG: Read failed, Wire.endTransmission returned: ");
        Serial.println(result, DEC);
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
        Serial.println("DEBUG: Read failed, unexpected amount of data");
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
    i2c_sensor::write_many(address, 1, &value);
    /*
    Wire.beginTransmission(device_address);
    Wire.send(address);
    Wire.send(value);
    byte result = Wire.endTransmission();
    if (result > 0)
    {
        Serial.print("DEBUG: Write failed, Wire.endTransmission returned: ");
        Serial.println(result, DEC);
        return false;
    }
    return true;
    */
}

boolean i2c_sensor::write_many(byte address, byte num, byte *source)
{
    Wire.beginTransmission(device_address);
    Wire.send(address);
    Wire.send(source, num);
    byte result = Wire.endTransmission();
    if (result > 0)
    {
        Serial.print("DEBUG: Write failed, Wire.endTransmission returned: ");
        Serial.println(result, DEC);
        return false;
    }
    return true;
}


void i2c_sensor::dump_registers(byte addr_start, byte addr_end)
{
    byte tmp;
    for (byte addr = addr_start; addr <= addr_end; addr++)
    {
        i2c_sensor::read(addr, &tmp);
        Serial.print("dev 0x");
        Serial.print(device_address, HEX);
        Serial.print(" reg 0x");
        Serial.print(addr, HEX);
        Serial.print(" value: 0x");
        Serial.print(tmp, HEX);
        Serial.print("\tB");
        Serial.println(tmp, BIN);
    }
}


