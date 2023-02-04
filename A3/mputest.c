//Author: Misael Lopez
//References: https://www.electronicwings.com/raspberry-pi/mpu6050-accelerometergyroscope-interfacing-with-raspberry-pi
//Date: 01/31/23
//Description: Testing MPU6050 communication for EEP522 Assignment 2

#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>

#define MPU6050_ADDRESS 0x68
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_GYRO_XOUT_H 0x43
#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47

int main(int argc, char *argv[]) 
{
  	if(!bcm2835_init()) 
	{
    	printf("bcm2835_init failed. Are you running as root?\n");
    	return 1;
  	}
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(MPU6050_ADDRESS);

	char buf1[2];
	char accel_x[2]; 
	char accel_y[2];
	char accel_z[2];
	char gyro_x[2]; 
	char gyro_y[2]; 
	char gyro_z[2];

	//Initialize
	buf1[0] = SMPLRT_DIV; 
	buf1[1] = 0x07;
	printf("Write Code: %d \n", bcm2835_i2c_write(buf1, 2));	/* Write to sample rate register */
	buf1[0] = PWR_MGMT_1; 
	buf1[1] = 0x01;
	printf("Write Code: %d \n", bcm2835_i2c_write(buf1, 2)); /* Write to power management register */
	buf1[0] = CONFIG; 
	buf1[1] = 0x00;
	printf("Write Code: %d \n", bcm2835_i2c_write(buf1, 2)); /* Write to Configuration register */
	buf1[0] = GYRO_CONFIG; 
	buf1[1] = 24;
	printf("Write Code: %d \n", bcm2835_i2c_write(buf1, 2));	/* Write to Gyro Configuration register */
	buf1[0] = INT_ENABLE; 
	buf1[1] = 0x01;
	printf("Write Code: %d \n", bcm2835_i2c_write(buf1, 2));

	// Read acceleration values
	buf1[0] = MPU6050_REG_ACCEL_XOUT_H; 
	printf("Write Code: %d \n", bcm2835_i2c_write(buf1, 1));
	printf("Read Code: %d \n", bcm2835_i2c_read(accel_x, 2));
	printf("Read Code: %d \n", bcm2835_i2c_read(accel_y, 2));
	printf("Read Code: %d \n", bcm2835_i2c_read(accel_z, 2));

	// Read gyroscope values
	buf1[0] = MPU6050_REG_GYRO_XOUT_H; 
	printf("Write Code: %d \n", bcm2835_i2c_write(buf1, 1));
	printf("Read Code: %d \n", bcm2835_i2c_read( gyro_x, 2));
	printf("Read Code: %d \n", bcm2835_i2c_read(gyro_y, 2));
	printf("Read Code: %d \n", bcm2835_i2c_read(gyro_z, 2));

	//Close I2C
	bcm2835_i2c_end();
	bcm2835_close();

	//Print Results
	printf("Accel X: %d  Accel Y: %d  Accel Z: %d\n", accel_x, accel_y, accel_z);
	printf("Gyro X: %d  Gyro Y: %d  Gyro Z: %d\n", gyro_x, gyro_y, gyro_z);

	return 0;
}