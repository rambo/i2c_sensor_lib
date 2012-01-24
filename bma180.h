// safety againts double-include
#ifndef bma180_h
#define bma180_h
#include <Arduino.h>
#include "i2c_accelerometer.h"
#include <Wire.h>


class bma180 : public i2c_accelerometer
{
    public:
        bma180();
        ~bma180();
        void read_sensor_data();

        // Initialize the object and chip        
        void begin(uint8_t dev_addr, uint8_t wire_begin);
        // A funky way to handle optional arguments
        void begin(uint8_t dev_addr);    
        void begin();
        // do a soft-reset for the chip and re-read the config registers
        void reset();
        uint8_t set_new_data_interrupt(boolean enable);
        uint8_t set_smp_skip(boolean enable);
        uint8_t set_bandwidth(uint8_t bw);
        uint8_t set_range(uint8_t range);

    protected:
        uint8_t ee_w;
        void set_ee_w(uint8_t enabled);
        // 2 uint8_t endian marker
        // 6 uint8_t buffer for saving data read from the device
        // 2 uint8_t checksum in case there is a reset in the middle of a packet.
        int last_data_buffer[5];

};



#endif
// *********** END OF CODE **********
