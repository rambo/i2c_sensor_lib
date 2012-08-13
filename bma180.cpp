#include "bma180.h"

// Constructor
bma180::bma180() : i2c_accelerometer()
{
}

// Destructor
bma180::~bma180()
{
}

void bma180::begin(uint8_t dev_addr, uint8_t wire_begin)
{
    i2c_accelerometer::begin(dev_addr, wire_begin);
    // do a soft-reset of the chip
    bma180::reset();
}
// Funky way to handle default arguments
void bma180::begin(uint8_t dev_addr)
{
    bma180::begin(dev_addr, true);
}
void bma180::begin()
{
    bma180::begin(0x40, true);
}


void bma180::reset()
{
    // Reset the device to get it to "known" state
    bma180::write(0x10, 0xB6);
    delayMicroseconds(20);
}

// Shorthand helpers for writing various registers
void bma180::set_ee_w(uint8_t enable)
{
    if (enable)
    {
        bma180::write(0x0d, B00010000);
    }
    else
    {
        bma180::write(0x0d, B00000000);
    }
    ee_w = enable;
}

uint8_t bma180::set_range(uint8_t range)
{
    switch (range)
    {
        case B000: // 1g
        case B001: // 1.5g
        case B010: // 2g
        case B011: // 3g
        case B100: // 4g
        case B101: // 8g
        case B110: // 16g
            if (!ee_w)
            {
                 bma180::set_ee_w(true);
            }
            return i2c_sensor::read_modify_write(0x35, B11110001, range << 1);
          break;
        default:
          Serial.print(F("Invalid value B"));
          Serial.println(range, BIN);
          return false;
    }
}

uint8_t bma180::set_bandwidth(uint8_t bw)
{
    switch (bw)
    {
        case B0000: // 10Hz
        case B0001: // 20Hz
        case B0010: // 40Hz
        case B0011: // 75Hz
        case B0100: // 150Hz
        case B0101: // 300Hz
        case B0110: // 600Hz
        case B0111: // 1200Hz
        case B1000: // High-pass: 1Hz
        case B1001: // Band-pass: 0.2Hz - 300Hz
            if (!ee_w)
            {
                 bma180::set_ee_w(true);
            }
            return i2c_sensor::read_modify_write(0x20, B00001111, bw << 4);
            delayMicroseconds(10);
          break;
        default:
          Serial.print(F("Invalid value B"));
          Serial.println(bw, BIN);
          return false;
    }
}


uint8_t bma180::set_new_data_interrupt(boolean enable)
{
    if (!ee_w)
    {
         bma180::set_ee_w(true);
    }
    return i2c_sensor::read_modify_write(0x21, B11111101, (enable & B00000001) << 1);
}

uint8_t bma180::set_smp_skip(boolean enable)
{
    if (!ee_w)
    {
         bma180::set_ee_w(true);
    }
    return i2c_sensor::read_modify_write(0x35, B1111110, (enable & B00000001));
}


void bma180::read_sensor_data()
{
    // TODO: Use only 3 item array, the checksum and the endiannes tag (copied from an old example) are useless here
    int last_data_buffer[5] = {0x8081, 0, 0, 0, 0};
    // PONDER: Why does not bma180::read_many work here ?
    i2c_sensor::read_many(0x02, 6, (uint8_t*)(last_data_buffer+1)); // Cast the pointer to the first element of the array to a byte and increment by one
    // Calculate checksum
    last_data_buffer[4] = last_data_buffer[1] + last_data_buffer[2] + last_data_buffer[3];
    // Remove the status bits from the data values
    last_data_buffer[1] = last_data_buffer[1] >> 2;
    last_data_buffer[2] = last_data_buffer[2] >> 2;
    last_data_buffer[3] = last_data_buffer[3] >> 2;
    
    i2c_accelerometer::push_to_smoothing_buffer(last_data_buffer[1], last_data_buffer[2], last_data_buffer[3]);
}



