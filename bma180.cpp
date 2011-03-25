#include "bma180.h"

// Constructor
bma180::bma180() : i2c_accelerometer()
{
}

// Destructor
bma180::~bma180()
{
}

// Funky way to handle default arguments
void bma180::begin(byte dev_addr, boolean wire_begin)
{
    i2c_accelerometer::begin(dev_addr, wire_begin);
    // TODO: Make a single pointer-array and use read_many to access consequtive registers
    bma180::read(0x20, &bw_tcs);
    bma180::read(0x21, &ctrl_reg_3);
    // TODO: Use read_many instead to avoid drilling through the methods ??
    bma180::read(0x35, &offset_lsb1);
}
void bma180::begin(byte dev_addr=0x40)
{
    bma180::begin(dev_addr, true);
}
void bma180::begin()
{
    bma180::begin(0x40, true);
}

void bma180::set_ee_w(boolean enable)
{
    if (enable)
    {
        bma180::write(0x0d, B00010000);
    }
    else
    {
        bma180::write(0x0d, B00010000);
    }
    ee_w = enable;
}


void bma180::write_ctrl_reg_3()
{
    if (!ee_w)
    {
         bma180::set_ee_w(true);
    }
    bma180::write(0x21, ctrl_reg_3);

}
void bma180::write_bw_tcs()
{
    if (!ee_w)
    {
         bma180::set_ee_w(true);
    }
    bma180::write(0x20, bw_tcs);
}
void bma180::write_offset_lsb1()
{
    if (!ee_w)
    {
         bma180::set_ee_w(true);
    }
    bma180::write(0x35, offset_lsb1);
}


// TODO: Use a byte value to conserve memory
void bma180::set_bandwidth(byte bw)
{
    // Shift the bw mask so we don't need to type all them dam bits
    if (bw <= 15)
    {
        bw = bw << 4;
    }
    //byte bw_tcs_mask = bw & B00001111
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
            bw_tcs &= (bw & B00001111);
          break;
        default:
          return;
    }

    bma180::write_bw_tcs();
}

void bma180::set_new_data_interrupt(boolean enable)
{
    if (enable)
    {
        ctrl_reg_3 |= B00000010;
    }
    else
    {
        ctrl_reg_3 &= B11111101;
    }
    bma180::write_ctrl_reg_3();
}

void bma180::set_smp_skip(boolean enable)
{
    if (enable)
    {
        offset_lsb1 |= B00000001;
    }
    else
    {
        offset_lsb1 &= B11111110;
    }
    bma180::write_offset_lsb1();
}


void bma180::read_sensor_data()
{
    // TODO: Use only 3 item array, the checksum and the endiannes tag (copied from an old example) are useless here
    int last_data_buffer[5] = {0x8081, 0, 0, 0, 0};
    // PONDER: Why does not bma180::read_many work here ?
    i2c_sensor::read_many(0x02, 6, (byte*)(last_data_buffer+1)); // Cast the pointer to the first element of the array to a byte and increment by one
    // Calculate checksum
    last_data_buffer[4] = last_data_buffer[1] + last_data_buffer[2] + last_data_buffer[3];
    // Remove the status bits from the data values
    last_data_buffer[1] = last_data_buffer[1] >> 2;
    last_data_buffer[2] = last_data_buffer[2] >> 2;
    last_data_buffer[3] = last_data_buffer[3] >> 2;
    
    i2c_accelerometer::push_to_smoothing_buffer(last_data_buffer[1], last_data_buffer[2], last_data_buffer[3]);
}



