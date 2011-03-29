#include "dummy_accelerometer.h"

// Constructor
dummy_accelerometer::dummy_accelerometer() : i2c_accelerometer()
{
}

// Destructor
dummy_accelerometer::~dummy_accelerometer()
{
}

void dummy_accelerometer::begin(byte dev_addr, boolean wire_begin)
{
//    i2c_accelerometer::begin(dev_addr, wire_begin);
    // do a soft-reset of the chip
    dummy_accelerometer::reset();
}
// Funky way to handle default arguments
void dummy_accelerometer::begin(byte dev_addr)
{
    dummy_accelerometer::begin(dev_addr, true);
}
void dummy_accelerometer::begin()
{
    dummy_accelerometer::begin(0x40, true);
}

void dummy_accelerometer::reset()
{
    // Reset the device to get it to "known" state
    bw_tcs = ctrl_reg_3 = offset_lsb1 = 0;
    dummy_accelerometer::write(0x10, 0xB6);
    delayMicroseconds(20);
    
    // TODO: Make a single pointer-array and use read_many to access consequtive registers ?
    dummy_accelerometer::read(0x20, &bw_tcs);
    dummy_accelerometer::read(0x21, &ctrl_reg_3);
    // WTF: If I do not read the offset_lsb1 values here the later manipulation methods will get wrong value!
    Serial.print("dummy_accelerometer::reset(): offset_lsb1 before: B");
    Serial.println(offset_lsb1, BIN);
    dummy_accelerometer::read_many(0x35, 1, &offset_lsb1);
    Serial.print("dummy_accelerometer::reset(): offset_lsb1 after: B");
    Serial.println(offset_lsb1, BIN);
}

// Shorthand helpers for writing various registers
void dummy_accelerometer::set_ee_w(boolean enable)
{
    if (enable)
    {
        dummy_accelerometer::write(0x0d, B00010000);
    }
    else
    {
        dummy_accelerometer::write(0x0d, B00000000);
    }
    ee_w = enable;
}
void dummy_accelerometer::write_ctrl_reg_3()
{
    if (!ee_w)
    {
         dummy_accelerometer::set_ee_w(true);
    }
    dummy_accelerometer::write(0x21, ctrl_reg_3);

}
void dummy_accelerometer::write_bw_tcs()
{
    if (!ee_w)
    {
         dummy_accelerometer::set_ee_w(true);
    }
    dummy_accelerometer::write(0x20, bw_tcs);
}
void dummy_accelerometer::write_offset_lsb1()
{
    if (!ee_w)
    {
         dummy_accelerometer::set_ee_w(true);
    }
    dummy_accelerometer::write(0x35, offset_lsb1);
}


void dummy_accelerometer::set_range(byte range)
{
    Serial.print("range: B");
    Serial.println(range, BIN);
    Serial.print("dummy_accelerometer::set_range(): offset_lsb1 before: B");
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
    Serial.print("dummy_accelerometer::set_range(): offset_lsb1 after: B");
    Serial.println(offset_lsb1, BIN);
    dummy_accelerometer::write_offset_lsb1();
}

void dummy_accelerometer::set_bandwidth(byte bw)
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
    dummy_accelerometer::write_bw_tcs();
    delayMicroseconds(10);
}


void dummy_accelerometer::set_new_data_interrupt(boolean enable)
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
    dummy_accelerometer::write_ctrl_reg_3();
}

void dummy_accelerometer::set_smp_skip(boolean enable)
{
    Serial.print("dummy_accelerometer::set_smp_skip(): offset_lsb1 before: B");
    Serial.println(offset_lsb1, BIN);
    if (enable)
    {
        offset_lsb1 |= B00000001;
    }
    else
    {
        offset_lsb1 &= B11111110;
    }
    Serial.print("dummy_accelerometer::set_smp_skip(): offset_lsb1 after: B");
    Serial.println(offset_lsb1, BIN);
    dummy_accelerometer::write_offset_lsb1();
}


void dummy_accelerometer::read_sensor_data()
{
	
    // TODO: Use only 3 item array, the checksum and the endiannes tag (copied from an old example) are useless here
    int last_data_buffer[5] = {0x8081, 0x800, 0x800, 0x800, 0};
	/*
    // PONDER: Why does not dummy_accelerometer::read_many work here ?
    dummy_accelerometer::read_many(0x02, 6, (byte*)(last_data_buffer+1)); // Cast the pointer to the first element of the array to a byte and increment by one
    // Calculate checksum
    last_data_buffer[4] = last_data_buffer[1] + last_data_buffer[2] + last_data_buffer[3];
    // Remove the status bits from the data values
    last_data_buffer[1] = last_data_buffer[1] >> 2;
    last_data_buffer[2] = last_data_buffer[2] >> 2;
    last_data_buffer[3] = last_data_buffer[3] >> 2;
    */
	
    i2c_accelerometer::push_to_smoothing_buffer(last_data_buffer[1], last_data_buffer[2], last_data_buffer[3]);
}

/*
byte dummy_accelerometer::read(byte address)
{
    return 0;
}
*/

boolean dummy_accelerometer::read(byte address, byte *target) {
  return true;
}


boolean dummy_accelerometer::read_many(byte address, byte req_num, byte *target)
{
	return true;
}


boolean dummy_accelerometer::write(byte address, byte value)
{
    Serial.println("dummy_accelerometer::write");
    return true;
}

boolean dummy_accelerometer::write_many(byte address, byte num, byte *source)
{
    Serial.println("dummy_accelerometer::write_many");
    return true;
}


void dummy_accelerometer::dump_registers(byte addr_start, byte addr_end)
{
    Serial.println("dummy_accelerometer::dump_registers");
    return;
}

