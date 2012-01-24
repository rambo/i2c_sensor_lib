// safety againts double-include
#ifndef dummy_accelerometer_h
#define dummy_accelerometer_h
#include <Arduino.h>
#include "i2c_accelerometer.h"
#include <Wire.h>


class dummy_accelerometer : public i2c_accelerometer
{
    public:
        dummy_accelerometer();
        ~dummy_accelerometer();
        void read_sensor_data();

        // Initialize the object and chip        
        void begin(uint8_t dev_addr, uint8_t wire_begin);
        // A funky way to handle optional arguments
        void begin(uint8_t dev_addr);    
        void begin();
        // do a soft-reset for the chip and re-read the config registers
        void reset();
        void set_new_data_interrupt(uint8_t enable);
        void set_smp_skip(uint8_t enable);
        void set_bandwidth(uint8_t bw);
        void set_range(uint8_t range);

		// Override all i2c methods
//        uint8_t read(byte address);
        // Read single uint8_t to a referred target (calls read_many internally)
        uint8_t read(uint8_t address, byte *target);
        // Read N uint8_ts to a target (usually an array)
        uint8_t read_many(uint8_t address, byte num, byte *target);
        // Helper to write a single uint8_t value (calls write_many internally)
        uint8_t write(uint8_t address, byte value);
        // Write N values from a target (usually an array)
        uint8_t write_many(uint8_t address, byte num, byte *source);

        void dump_registers(uint8_t addr_start, byte addr_end);

    protected:
        uint8_t ee_w;
        void set_ee_w(uint8_t enabled);
        // 2 uint8_t endian marker
        // 6 uint8_t buffer for saving data read from the device
        // 2 uint8_t checksum in case there is a reset in the middle of a packet.
        volatile int last_data_buffer[5];
        uint8_t ctrl_reg_3;
        void write_ctrl_reg_3();
        uint8_t bw_tcs;
        void write_bw_tcs();
        uint8_t offset_lsb1;
        void write_offset_lsb1();

};



#endif
// *********** END OF CODE **********
