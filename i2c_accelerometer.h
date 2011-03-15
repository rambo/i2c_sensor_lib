// safety againts double-include
#ifndef i2c_accelerometer_h
#define i2c_accelerometer_h
#include <WProgram.h> 
#include "i2c_sensor.h"
#include <Wire.h>

class i2c_accelerometer : public i2c_sensor
{
    public:
        i2c_accelerometer();
        ~i2c_accelerometer();
        void begin(byte dev_addr, boolean wire_begin);
    
    protected:
        int smooth_buffer[5][3];
        int last_data_buffer[3];
};

#endif
// *********** END OF CODE **********
