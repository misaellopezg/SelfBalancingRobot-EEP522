//Created by: Misa Lopez
//Date: 02/03/23
//Change log: 
//  
//
//
#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include "mpu6050.h"
#include <math.h>
#include "l298n.h"

#define MPU6050_ADDRESS 0x68

int main(int argc, char *argv[]) 
{
	/*Set up BCM2835 for I2C*/
  	if(!bcm2835_init()) 
	{
    	printf("bcm2835_init failed. Are you running as root?\n");
    	return 1;
  	}
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS);
	

	/*Create MPU6050 Sensor*/
	mpu6050 mympu; 
	//mympu.reset();
	mympu.initialize(); 

	uint8_t d_ID; //testing device ID
	int16_t ax; //accel x axis
	int16_t ay; //accel y axis
	int16_t az; //accel z axis
	int16_t gx; //gyro x axis
	int16_t gy; //gyro y axis
	int16_t gz; //gyro z axis

	float angle; //calculate angle

	d_ID = mympu.getDeviceID(); 
	printf("Device ID: %d \n", d_ID); 
	while(1)
	{
		//mympu.getAccelX(&ax); 
		mympu.getAccelY(&ay); 
		mympu.getAccelZ(&az); 
		mympu.getGyroX(&gx);

		angle = atan2((double)ay,(double)az)*(180/M_PI);
		printf("Accel Angle: %f \n", angle); 

	}
	bcm2835_i2c_end();
	bcm2835_close();

	return 0;
}