#include "i2c_sensor.h"

// Constructor
i2c_sensor::i2c_sensor()
{
}

// Destructor
i2c_sensor::~i2c_sensor()
{
}

void i2c_sensor::begin(uint8_t dev_addr, uint8_t wire_begin)
{
    device_address = dev_addr;
    if (wire_begin)
    {
        Wire.begin();
    }
}

void i2c_sensor::get_last_raw(int *target)
{
}
void i2c_sensor::get_last_smoothed(int *target)
{
}
void i2c_sensor::read_sensor_data()
{
}


uint8_t i2c_sensor::read(uint8_t address, byte *target)
{
    return i2c_sensor::read_many(address, 1, target);
}
uint8_t i2c_sensor::read(byte address)
{
    uint8_t target;
    i2c_sensor::read_many(address, 1, &target);
    return target;
}



uint8_t i2c_sensor::read_many(uint8_t address, byte req_num, byte *target)
{
    Wire.beginTransmission(device_address);
    Wire.send(address);
    uint8_t result = Wire.endTransmission();
    if (result > 0)
    {
        Serial.print(F("DEBUG: Read failed, Wire.endTransmission returned: "));
        Serial.println(result, DEC);
        return false;
    }
    Wire.requestFrom(device_address, req_num);
    uint8_t recv_num =  Wire.available();
    if (recv_num != req_num)
    {
        // Unexpected amount of data to be received, clear the buffers and return failure
        while (recv_num-- > 0)
        {
            Wire.receive();
        }
        Serial.println(F("DEBUG: Read failed, unexpected amount of data"));
        return false;
    }
    while(recv_num-- > 0)
    {
        // First assign the return of Wire.receive to where the pointer points to, then incement the pointer (so in next iteration we write to correct place)
        *(target++) = Wire.receive();
    }
    return true;
}


uint8_t i2c_sensor::write(uint8_t address, byte value)
{
    return i2c_sensor::write_many(address, 1, &value);
}

uint8_t i2c_sensor::write_many(uint8_t address, byte num, byte *source)
{
    Wire.beginTransmission(device_address);
    Wire.send(address);
    Wire.send(source, num);
    uint8_t result = Wire.endTransmission();
    if (result > 0)
    {
        Serial.print(F("DEBUG: Write failed, Wire.endTransmission returned: "));
        Serial.println(result, DEC);
        return false;
    }
    return true;
}

uint8_t i2c_sensor::read_modify_write(uint8_t address, byte mask, byte value)
{
    uint8_t tmp;
    if (!i2c_sensor::read_many(address, 1, &tmp))
    {
        return false;
    }
    /*
    Serial.print(F("BEFORE: reg 0x"));
    Serial.print(address, HEX);
    Serial.print(F(" value: 0x"));
    Serial.print(tmp, HEX);
    Serial.print(F("\tB"));
    Serial.println(tmp, BIN);
    Serial.print(F("MASK: B"));
    Serial.print(mask, BIN);
    Serial.print(F("\tVALUE: B"));
    Serial.println(value, BIN);
    */
    tmp = (tmp & mask) | value;
    /*
    Serial.print(F("AFTER: reg 0x"));
    Serial.print(address, HEX);
    Serial.print(F(" value: 0x"));
    Serial.print(tmp, HEX);
    Serial.print(F("\tB"));
    Serial.println(tmp, BIN);
    */
    return i2c_sensor::write_many(address, 1, &tmp);
}


void i2c_sensor::dump_registers(uint8_t addr_start, byte addr_end)
{
    uint8_t tmp;
    for (uint8_t addr = addr_start; addr <= addr_end; addr++)
    {
        if (!i2c_sensor::read(addr, &tmp))
        {
            continue;
        }
        Serial.print(F("dev 0x"));
        Serial.print(device_address, HEX);
        Serial.print(F(" reg 0x"));
        Serial.print(addr, HEX);
        Serial.print(F(" value: 0x"));
        Serial.print(tmp, HEX);
        Serial.print(F("\tB"));
        Serial.println(tmp, BIN);
    }
}




