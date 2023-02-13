//Created by: Misa Lopez
//Date: 02/03/23
//Change log: 
//  
//
//
#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "mpu6050.h"
#include <math.h>
#include "l298n.h"
#include <time.h>

#define MPU6050_ADDRESS 0x68

#define PIN17 RPI_BPLUS_GPIO_J8_11 //GPIO 0
#define PIN27 RPI_BPLUS_GPIO_J8_13 //GPIO 2
#define PIN18 RPI_BPLUS_GPIO_J8_12   // GPIO 12 PWN

#define SAMPLE_TIME 0.02; 

long mapval(long x, long in_min, long in_max, long out_min, long out_max);

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

	float accelAngle; //calculate accel angle
	float gyroAngle; //calculate gyro angle
	float curAngle; //current angle measrued using complementary filter
	float prevAngle; //previous angle measured
	accelAngle = 0; 
	gyroAngle = 0; 
	curAngle = 0; 
	prevAngle = 0; 

	/*Create H Bridge controller for motors*/
	l298n myhbridge(PIN17, PIN27, PIN18); 

	/*Test MPU*/
	d_ID = mympu.getDeviceID(); 
	printf("Device ID: %d \n", d_ID); 

	/*Define Timer variables*/
	clock_t t; 
	t = clock();  
	double time_taken; 

	/*Define other variables*/
	
	while(1)
	{ 
		mympu.getAccelY(&ay); 
		mympu.getAccelZ(&az); 
		mympu.getGyroX(&gx);
		accelAngle = atan2((double)ay,(double)az)*(180/M_PI);

		//Execute every SAMPLE_TIME seconds
		time_taken = ((double)(clock()-t))/CLOCKS_PER_SEC;
		if(time_taken > 0.02)
		{
			long gxrate = mapval(gx, -32768, 32767, -250, 250);
			gyroAngle = gyroAngle + (float)gxrate*time_taken; 
			float curAngle = 0.0066*(prevAngle + gyroAngle) + (1.0-0.0066)*(accelAngle);
			prevAngle = curAngle; 
			printf("Angle: %f\n", curAngle); 
			//printf("Time elapsed: %f s\n", time_taken); 
			t = clock(); 
		}

	}
	bcm2835_i2c_end();
	bcm2835_close();

	return 0;
}


long mapval(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}