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

    protected:
        // 2 byte endian marker (ie single int)
        // 6 byte buffer for saving data read from the device (ie 3 ints)
        // 2 byte checksum in case there is a reset in the middle of a packet. (ie single int)
        int last_data_buffer[5];
};



#endif
// *********** END OF CODE **********
