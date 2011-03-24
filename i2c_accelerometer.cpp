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
    Serial.println("DEBUG-BEGIN: i2c_accelerometer::push_to_smoothing_buffer()");
    Serial.print(val_x, DEC);
    Serial.print('\t');
    Serial.print(val_y, DEC);
    Serial.print('\t');
    Serial.println(val_z, DEC);
    */
    // Scrap old values if buffer fills
    // TODO: rewrite to check the return value of the enqueue ?
    if (smoothing_buffer[0].count() == I2C_ACCELEROMETER_SMOOTH_BUFFER_SIZE)
    {
        smoothing_buffer[0].dequeue();    
        smoothing_buffer[1].dequeue();
        smoothing_buffer[2].dequeue();
    }
    smoothing_buffer[0].enqueue(val_x);    
    smoothing_buffer[1].enqueue(val_y);
    smoothing_buffer[2].enqueue(val_z);
    //Serial.println("DEBUG-END: i2c_accelerometer::push_to_smoothing_buffer()");
}

void i2c_accelerometer::smooth()
{
    // Naive smoothing for now
    byte d = smoothing_buffer[0].count();
    byte div = d;
    int temp_buffer[3];
    Serial.print("Smoothing ");
    Serial.print(d, DEC);
    Serial.print(":\t");
    Serial.print(temp_buffer[0], DEC);
    Serial.print('\t');
    Serial.print(temp_buffer[1], DEC);
    Serial.print('\t');
    Serial.println(temp_buffer[2], DEC);
    while (d-- > 0)
    {
        Serial.print("Smoothing ");
        Serial.print(d, DEC);
        Serial.print(":\t");
        temp_buffer[0] += smoothing_buffer[0].dequeue(); 
        temp_buffer[1] += smoothing_buffer[1].dequeue(); 
        temp_buffer[2] += smoothing_buffer[2].dequeue(); 
        Serial.print(temp_buffer[0], DEC);
        Serial.print('\t');
        Serial.print(temp_buffer[1], DEC);
        Serial.print('\t');
        Serial.println(temp_buffer[2], DEC);
    }
    smoothed_buffer[0] = smoothed_buffer[0] + temp_buffer[0] / (div+1); 
    smoothed_buffer[1] = smoothed_buffer[1] + temp_buffer[0] / (div+1); 
    smoothed_buffer[2] = smoothed_buffer[2] + temp_buffer[0] / (div+1); 
    /*
    smoothed_buffer[0] = temp_buffer[0] / div; 
    smoothed_buffer[1] = temp_buffer[0] / div; 
    smoothed_buffer[2] = temp_buffer[0] / div; 
    */
}

void i2c_accelerometer::get_last_smoothed(int *target)
{
    target[0] = smoothed_buffer[0];
    target[1] = smoothed_buffer[1];
    target[2] = smoothed_buffer[2];
}

