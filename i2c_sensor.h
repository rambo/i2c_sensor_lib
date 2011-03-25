// safety againts double-include
#ifndef i2c_sensor_h
#define i2c_sensor_h
#include <WProgram.h> 
#include <Wire.h>


class i2c_sensor
{
    public:
        i2c_sensor();
        ~i2c_sensor();
        void begin(byte dev_addr, boolean wire_begin);
        virtual void read_sensor_data();
        void get_last_raw(void *target);
        void get_last_smoothed(void *target);
        void get_buffer(void *target);

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

        void dump_registers(byte addr_start, byte addr_end);

    protected:
        byte device_address;
        // PONDER: These might be better not be defined here (if overloading is b0rked or something
        int smoothed_buffer;
        volatile int last_data_buffer;

};


#endif
// *********** END OF CODE **********
