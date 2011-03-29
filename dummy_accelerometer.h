// safety againts double-include
#ifndef dummy_accelerometer_h
#define dummy_accelerometer_h
#include <WProgram.h>
#include "i2c_accelerometer.h"
#include <Wire.h>


class dummy_accelerometer : public i2c_accelerometer
{
    public:
        dummy_accelerometer();
        ~dummy_accelerometer();
        void read_sensor_data();

        // Initialize the object and chip        
        void begin(byte dev_addr, boolean wire_begin);
        // A funky way to handle optional arguments
        void begin(byte dev_addr);    
        void begin();
        // do a soft-reset for the chip and re-read the config registers
        void reset();
        void set_new_data_interrupt(boolean enable);
        void set_smp_skip(boolean enable);
        void set_bandwidth(byte bw);
        void set_range(byte range);

		// Override all i2c methods
//        byte read(byte address);
        // Read single byte to a referred target (calls read_many internally)
        boolean read(byte address, byte *target);
        // Read N bytes to a target (usually an array)
        boolean read_many(byte address, byte num, byte *target);
        // Helper to write a single byte value (calls write_many internally)
        boolean write(byte address, byte value);
        // Write N values from a target (usually an array)
        boolean write_many(byte address, byte num, byte *source);

        void dump_registers(byte addr_start, byte addr_end);

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
