#include "bma180.h"

// Constructor
bma180::bma180() : i2c_accelerometer()
{
}

// Destructor
bma180::~bma180()
{
}

void bma180::begin(byte dev_addr, boolean wire_begin)
{
    i2c_accelerometer::begin(dev_addr, wire_begin);

    // Reset the device to get it to "known" state
    bw_tcs = ctrl_reg_3 = offset_lsb1 = 0;
    bma180::write(0x10, 0xB6);
    delayMicroseconds(20);
    
    // TODO: Make a single pointer-array and use read_many to access consequtive registers ?
    bma180::read(0x20, &bw_tcs);
    bma180::read(0x21, &ctrl_reg_3);
    // TODO: Use read_many instead to avoid drilling through the methods ??
    Serial.print("bma180::begin(): offset_lsb1 before: B");
    Serial.println(offset_lsb1, BIN);
    bma180::read(0x35, &offset_lsb1);
    Serial.print("bma180::begin(): offset_lsb1 after: B");
    Serial.println(offset_lsb1, BIN);
}
// Funky way to handle default arguments
void bma180::begin(byte dev_addr)
{
    bma180::begin(dev_addr, true);
}
void bma180::begin()
{
    bma180::begin(0x40, true);
}


// Shorthand helpers for writing various registers
void bma180::set_ee_w(boolean enable)
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


void bma180::set_range(byte range)
{
    Serial.print("range: B");
    Serial.println(range, BIN);
    Serial.print("bma180::set_range(): offset_lsb1 before: B");
    Serial.println(offset_lsb1, BIN);
    switch (range)
    {
        case B000: // 1g
        case B001: // 1.5g
        case B010: // 2g
        case B011: // 3g
        case B100: // 4g
        case B101: // 8g
        case B110: // 16g
            offset_lsb1 = (offset_lsb1 & B11110001) | range << 1;
          break;
        default:
          Serial.print("Invalid value B");
          Serial.println(range, BIN);
          return;
    }
    Serial.print("bma180::set_range(): offset_lsb1 after: B");
    Serial.println(offset_lsb1, BIN);
    bma180::write_offset_lsb1();
}

void bma180::set_bandwidth(byte bw)
{
    /*
    Serial.print("bw: B");
    Serial.println(bw, BIN);

    Serial.print("bw_tcs before: B");
    Serial.println(bw_tcs, BIN);
    */
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
            bw_tcs = (bw_tcs & B00001111) | bw << 4;
          break;
        default:
          Serial.print("Invalid value B");
          Serial.println(bw, BIN);
          return;
    }
    /*
    Serial.print("bw_tcs after: B");
    Serial.println(bw_tcs, BIN);
    */
    bma180::write_bw_tcs();
    delayMicroseconds(10);
}


void bma180::set_new_data_interrupt(boolean enable)
{
    /*
    Serial.print("ctrl_reg_3 before: B");
    Serial.println(ctrl_reg_3, BIN);
    */
    if (enable)
    {
        ctrl_reg_3 |= B00000010;
    }
    else
    {
        ctrl_reg_3 &= B11111101;
    }
    /*
    Serial.print("ctrl_reg_3 after: B");
    Serial.println(ctrl_reg_3, BIN);
    */
    bma180::write_ctrl_reg_3();
}

void bma180::set_smp_skip(boolean enable)
{
    Serial.print("bma180::set_smp_skip(): offset_lsb1 before: B");
    Serial.println(offset_lsb1, BIN);
    if (enable)
    {
        offset_lsb1 |= B00000001;
    }
    else
    {
        offset_lsb1 &= B11111110;
    }
    Serial.print("bma180::set_smp_skip(): offset_lsb1 after: B");
    Serial.println(offset_lsb1, BIN);
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



