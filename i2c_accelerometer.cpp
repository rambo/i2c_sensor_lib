#include "i2c_accelerometer.h"

// Constructor
i2c_accelerometer::i2c_accelerometer() : i2c_sensor()
{
}

// Destructor
i2c_accelerometer::~i2c_accelerometer()
{
}

void i2c_accelerometer::begin(byte dev_addr, boolean wire_begin)
{
    i2c_sensor::begin(dev_addr, wire_begin);
}

void i2c_accelerometer::push_to_smoothing_buffer(int val_x, int val_y, int val_z)
{
    /*
    Serial.print("push_to_smoothing_buffer()\t");
    Serial.print(val_x, DEC);
    Serial.print('\t');
    Serial.print(val_y, DEC);
    Serial.print('\t');
    Serial.println(val_z, DEC);
    */
    // Scrap old values if buffer fills
    // TODO: rewrite to check the return value of the enqueue (should save one method call...)
    if (smoothing_buffer[0].count() == I2C_ACCELEROMETER_SMOOTH_BUFFER_SIZE)
    {
        smoothing_buffer[0].dequeue();    
        smoothing_buffer[1].dequeue();
        smoothing_buffer[2].dequeue();
    }
    smoothing_buffer[0].enqueue(val_x);    
    smoothing_buffer[1].enqueue(val_y);
    smoothing_buffer[2].enqueue(val_z);
}

void i2c_accelerometer::smooth()
{
    // Naive smoothing for now
    byte d = smoothing_buffer[0].count();
    byte div = d;
    //long temp_buffer[3] = {0,0,0};
    long temp_buffer[3] = {smoothed_buffer[0], smoothed_buffer[1], smoothed_buffer[2]};
    while (d-- > 0)
    {
        temp_buffer[0] += smoothing_buffer[0].dequeue(); 
        temp_buffer[1] += smoothing_buffer[1].dequeue(); 
        temp_buffer[2] += smoothing_buffer[2].dequeue(); 
    }
    /**
     * Use this if not pre-seeding temp_buffer with the previous smoothed buffer
    smoothed_buffer[0] = temp_buffer[0] / div; 
    smoothed_buffer[1] = temp_buffer[1] / div; 
    smoothed_buffer[2] = temp_buffer[2] / div; 
    */
    smoothed_buffer[0] = temp_buffer[0] / (div+1); 
    smoothed_buffer[1] = temp_buffer[1] / (div+1); 
    smoothed_buffer[2] = temp_buffer[2] / (div+1); 
}

void i2c_accelerometer::get_last_smoothed(int *target)
{
    // The target is an array so no need to specify the * again, it's referred automagically
    target[0] = smoothed_buffer[0];
    target[1] = smoothed_buffer[1];
    target[2] = smoothed_buffer[2];
}

