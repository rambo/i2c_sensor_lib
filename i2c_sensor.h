// safety againts double-include
#ifndef i2c_sensor_h
#define i2c_sensor_h
#include <Arduino.h> 
#include <Wire.h>


class i2c_sensor
{
    public:
        i2c_sensor();
        ~i2c_sensor();
        void begin(uint8_t dev_addr, uint8_t wire_begin);
        virtual void read_sensor_data();
        virtual void get_last_raw(int *target);
        virtual void get_last_smoothed(int *target);

        // A Very shorthand helper for reading single uint8_t (NOTE: does not do error-checking!)
        uint8_t read(byte address);
        // Read single uint8_t to a referred target (calls read_many internally)
        uint8_t read(uint8_t address, byte *target);
        // Read N uint8_ts to a target (usually an array)
        uint8_t read_many(uint8_t address, byte num, byte *target);
        // Helper to write a single uint8_t value (calls write_many internally)
        uint8_t write(uint8_t address, byte value);
        // Write N values from a target (usually an array)
        uint8_t write_many(uint8_t address, byte num, byte *source);
        uint8_t read_modify_write(uint8_t address, byte mask, byte value);

        void dump_registers(uint8_t addr_start, byte addr_end);

        // TODO: Make method(s) to read-modify-write a register with a specific mask, at some point keeping copies of all registers we wish to manipulate in memory will get expensive

    protected:
        uint8_t device_address;
        // PONDER: These might be better not be defined here (if overloading is b0rked or something
        int smoothed_buffer;
        // This probably does not need to be volatile since we can't do I2C operations inside interrupts anyway...
        volatile int last_data_buffer;

};


#endif
// *********** END OF CODE **********
