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
        void begin(byte dev_addr, boolean wire_begin);
        virtual void read_sensor_data();
        virtual void get_last_raw(int *target);
        virtual void get_last_smoothed(int *target);

        // A Very shorthand helper for reading single byte (NOTE: does not do error-checking!)
        byte read(byte address);
        // Read single byte to a referred target (calls read_many internally)
        boolean read(byte address, byte *target);
        // Read N bytes to a target (usually an array)
        boolean read_many(byte address, byte num, byte *target);
        // Helper to write a single byte value (calls write_many internally)
        boolean write(byte address, byte value);
        // Write N values from a target (usually an array)
        boolean write_many(byte address, byte num, byte *source);
        boolean read_modify_write(byte address, byte mask, byte value);

        void dump_registers(byte addr_start, byte addr_end);

        // TODO: Make method(s) to read-modify-write a register with a specific mask, at some point keeping copies of all registers we wish to manipulate in memory will get expensive

    protected:
        byte device_address;
        // PONDER: These might be better not be defined here (if overloading is b0rked or something
        int smoothed_buffer;
        // This probably does not need to be volatile since we can't do I2C operations inside interrupts anyway...
        volatile int last_data_buffer;

};


#endif
// *********** END OF CODE **********
