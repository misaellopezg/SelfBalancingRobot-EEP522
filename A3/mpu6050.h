//Created by: Misa Lopez
//Date: 02/03/23
//Based on ElectronicCats/mpu6050 git repository and mpu6050 documentation
//Change log: 
//  
//
//
#ifndef MPU6050_H
#define MPU6050_H

class mpu6050
{
    public:
        void reset();//reset sensor
        void initialize(); //Initialize sensor
        void setFullScaleGyroRange(); // configure the gyroscopes’ full scale range
        void setFullScaleAccelRange(); // configure the accelerometer full scale range
        //void setSleepEnabled(); // set sleep bit status
        uint8_t getDeviceID(); // used to get device i2c ID
        void getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz); // used to get raw 6-axis motion;
        void getAccelX(int16_t *ax);// get X axis acceleration
        void getAccelY(int16_t *ay);// get y axis acceleration
        void getAccelZ(int16_t *az);// get z axis acceleration
        void getGyroX(int16_t *gx);// get X axis gyro
        void getGyroY(int16_t *gy);// get y axis gyro
        void getGyroZ(int16_t *gz);// get z axis gyro
};
#endif