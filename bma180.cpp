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
    int last_data_buffer[5] = {0x8081, 0, 0, 0, 0};
    i2c_accelerometer::begin(dev_addr, wire_begin);
    // TODO: Read the control reg values so our mask ops give expected results
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
    Wire.beginTransmission(device_address);
    Wire.send(0x0d);
    if (enable)
    {
        Wire.send(B00010000);
    }
    else
    {
        Wire.send(B00000000);
    }
    Wire.endTransmission();
    ee_w = enable;
}


// TODO: Refactor to use general set_register methods
void bma180::set_ctrl_reg_3()
{
    if (!ee_w)
    {
         bma180::set_ee_w(true);
    }
    Wire.beginTransmission(device_address);
    Wire.send(0x21);
    Wire.send(ctrl_reg_3);
    Wire.endTransmission();
}
void bma180::set_bw_tcs()
{
    if (!ee_w)
    {
         bma180::set_ee_w(true);
    }
    Wire.beginTransmission(device_address);
    Wire.send(0x20);
    Wire.send(bw_tcs);
    Wire.endTransmission();
}

void bma180::set_bandwidth(int bw)
{
    switch (bw)
    {
        case 10:
          bw_tcs &= B00001111;
          break;
        case 20:
          bw_tcs &= B00011111;
          break;
        case 40:
          bw_tcs &= B00101111;
          break;
        case 75:
          bw_tcs &= B00111111;
          break;
        case 150:
          bw_tcs &= B01001111;
          break;
        case 300:
          bw_tcs &= B01011111;
          break;
        case 600:
          bw_tcs &= B01101111;
          break;
        case 1200:
          bw_tcs &= B01111111;
          break;
        default:
          return;
    }
    bma180::set_bw_tcs();
}

void bma180::set_new_data_interrupt(boolean enable)
{
    if (enable)
    {
        // set adv_int too
        //ctrl_reg_3 |= B00000110;
        // 
        ctrl_reg_3 |= B00000010;
    }
    else
    {
        ctrl_reg_3 &= B11111101;
    }
    bma180::set_ctrl_reg_3();
}


// TODO: Move the i2c stuff to parent class
void bma180::read_sensor_data()
{
    i2c_accelerometer::readFrom(0x02, 6, (byte*)(last_data_buffer+1));
    // Copied from the examples for ADXL345
    /*
    Wire.beginTransmission(device_address);
    Wire.send(0x02);
    Wire.endTransmission();
    
    byte read_bytes = 6;
    Wire.requestFrom(device_address, read_bytes);
    read_bytes = Wire.available(); //device may send less than requested (abnormal)
    byte *buff = (byte*)(last_data_buffer+1);
    // Wouldn't this be better with num-- alone on --num ?
    while (read_bytes-- > 0)
    {
        *(buff++) = Wire.receive();
    }
    */
    // Calculate checksum
    last_data_buffer[4] = last_data_buffer[1] + last_data_buffer[2] + last_data_buffer[3];
    last_data_buffer[1] = last_data_buffer[1] >> 2;
    last_data_buffer[2] = last_data_buffer[2] >> 2;
    last_data_buffer[3] = last_data_buffer[3] >> 2;
    
    i2c_accelerometer::push_to_smoothing_buffer(last_data_buffer[1], last_data_buffer[2], last_data_buffer[3]);
}



