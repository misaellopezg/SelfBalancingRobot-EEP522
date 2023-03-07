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
#include "pid.h"

#define MPU6050_ADDRESS 0x68 //MPU6050 address
#define PIN17 RPI_BPLUS_GPIO_J8_11 //GPIO 17
#define PIN27 RPI_BPLUS_GPIO_J8_13 //GPIO 27
#define PIN18 RPI_BPLUS_GPIO_J8_12   // GPIO 18 PWN

/*Mapping function 
Used to transform PID output to PWM val*/
long mapval(long x, long in_min, long in_max, long out_min, long out_max);

int main() 
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
	uint16_t pwmval; //pwm val for h bridge
	const uint16_t minPWM = 71; //set min PWM val for motors 
	const uint16_t maxPWM = 225; //set Max PWM val for motors

	/*Create PID Controller*/
	double input; 
	double output; 
	double setpoint; 
	setpoint = 0; //targeting 0 degrees
	PID mypid(&input, &output, &setpoint, 1.0, 2.0, 0.0); 

	/*Test MPU*/
	d_ID = mympu.getDeviceID(); 
	printf("Device ID: %d \n", d_ID); 

	/*Define Timer variables*/
	clock_t t; 
	t = clock();  
	unsigned long time_taken; 

	/*Define other variables*/
	
	while(1)
	{ 
		//Get acceleration values to calculate angle
		mympu.getAccelY(&ay); 
		mympu.getAccelZ(&az); 
		mympu.getGyroX(&gx);
		accelAngle = atan2((double)ay,(double)az)*(180/M_PI);

		//Execute PID loop every SAMPLE_TIME seconds
		time_taken = clock()-t;
		if(time_taken > 20000) 
		{
			curAngle = accelAngle; 
			input = curAngle; 
			mypid.computePID(); 
			printf("PID output: %f\n", output); 
			t = clock(); 
		}
		//Based on PID output, set motors to rotate in desired direction along with PWM val
		if(output < 0)
		{
			pwmval = (uint16_t)mapval(output*-1.0, 0, 255, minPWM, maxPWM); 
			myhbridge.counterclockwiseMotors();
			myhbridge.setPWM(pwmval); 
		}
		if(output >= 0)
		{
			pwmval = (uint16_t)mapval(output, 0, 255, minPWM, maxPWM); 
			myhbridge.clockwiseMotors(); 
			myhbridge.setPWM(pwmval);  
		}
	}
	bcm2835_i2c_end();
	bcm2835_close();

	return 0;
}

/*Mapping function 
Used to transform PID output to PWM val*/
long mapval(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}