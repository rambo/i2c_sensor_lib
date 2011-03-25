// safety againts double-include
#ifndef bma180_h
#define bma180_h
#include <WProgram.h>
#include "i2c_accelerometer.h"
#include <Wire.h>


class bma180 : public i2c_accelerometer
{
    public:
        bma180();
        ~bma180();
        void read_sensor_data();
        
        // A funky way to handle optional arguments
        void begin(byte dev_addr, boolean wire_begin);
        void begin(byte dev_addr);    
        void begin();
        void set_new_data_interrupt(boolean enable);
        void set_smp_skip(boolean enable);
        void set_bandwidth(byte bw);

    protected:
        boolean ee_w;
        void set_ee_w(boolean enabled);
        // 2 byte endian marker
        // 6 byte buffer for saving data read from the device
        // 2 byte checksum in case there is a reset in the middle of a packet.
        volatile int last_data_buffer[5];
        byte ctrl_reg_3;
        void write_ctrl_reg_3();
        byte bw_tcs;
        void write_bw_tcs();
        byte offset_lsb1;
        void write_offset_lsb1();

};



#endif
// *********** END OF CODE **********
