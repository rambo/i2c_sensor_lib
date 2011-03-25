// safety againts double-include
#ifndef i2c_accelerometer_h
#define i2c_accelerometer_h
#include <WProgram.h> 
#include "i2c_sensor.h"
#include <Wire.h>
#include <SimpleFIFO.h> 


// Care needs to be taken so that the temp buffer in the smooth method does not overflow
#ifndef I2C_ACCELEROMETER_SMOOTH_BUFFER_SIZE
#define I2C_ACCELEROMETER_SMOOTH_BUFFER_SIZE 5
#endif

class i2c_accelerometer : public i2c_sensor
{
    public:
        i2c_accelerometer();
        ~i2c_accelerometer();
        void begin(byte dev_addr, boolean wire_begin);
        void smooth();
        void get_last_smoothed(int *target);
    
    protected:
        SimpleFIFO<int,I2C_ACCELEROMETER_SMOOTH_BUFFER_SIZE> smoothing_buffer[3]; // One buffer for each channel
        //int smoothing_buffer[I2C_ACCELEROMETER_SMOOTH_BUFFER_SIZE][3];
        volatile int last_data_buffer[3];
        int smoothed_buffer[3];
        void push_to_smoothing_buffer(int val_x, int val_y, int val_z);
        
};

#endif
// *********** END OF CODE **********
