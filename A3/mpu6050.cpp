//Created by: Misa Lopez
//Date: 02/03/23
//Based on ElectronicCats/mpu6050 git repository and mpu6050 documentation
//Change log: 
//  
//
//
#include <stdio.h>
#include <stdint.h>
#include <bcm2835.h>
#include "mpu6050.h"
#include <unistd.h>

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define MPU6050_REG_GYRO_XOUT_H 0x43
#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47
#define WHO_AM_I 0X75

#define RESET_VAL 0x81
#define FS_SEL 0x18 

/*Reset sensor*/
/*DON'T FORGET TO WAIT 100 MS AFTER RESETTING*/
void mpu6050::reset()
{
	char buf[2];
	/*Resetting sensor values*/
	buf[0] = PWR_MGMT_1; 
	buf[1] = RESET_VAL; 
	printf("Reset PWR_MGMT_1 Code: %d \n", bcm2835_i2c_write(buf, 2));
	usleep(100000); 
}
/* Initialize Sensor*/
void mpu6050::initialize()
{
    char buf[2];
	/* Write to sample rate register */
    buf[0] = SMPLRT_DIV; 
	buf[1] = 0x00;
	printf("SMPLRT_DIV Register Code: %d \n", bcm2835_i2c_write(buf, 2));
	/* Write to power management register*/	
	buf[0] = PWR_MGMT_1; 
	buf[1] = 0x01; // wake up sensor, use gyroscope X clck
	printf("PWR_MGMT_1 Register Code: %d \n", bcm2835_i2c_write(buf, 2)); 
	/* Write to Configuration register */
	buf[0] = CONFIG; 
	buf[1] = 0x00;
	printf("CONFIG Register Code: %d \n", bcm2835_i2c_write(buf, 2)); 
	/* Write to Gyro Configuration register */
	buf[0] = GYRO_CONFIG; 
	buf[1] = FS_SEL; //24
	printf("GYRO_CONFIG Register Code: %d \n", bcm2835_i2c_write(buf, 2));
	/*INterrupt Enable*/
	buf[0] = INT_ENABLE; 
	buf[1] = 0x01;
	printf("INT_ENABLE Register Code: %d \n", bcm2835_i2c_write(buf, 2));
	/*Configure Gyro*/
	setFullScaleGyroRange(); 
	/*Configur Accelerometer*/
	setFullScaleAccelRange(); 
}
/*Config Gyro Range*/
void mpu6050::setFullScaleGyroRange()
{
	char buf[2]; 
	/*Write to Gyro Config register*/
	buf[0] = GYRO_CONFIG; 
	buf[1] = 0x0; //set to default 250 deg/sec
	printf("GYRO_CONFIG Register Code: %d \n", bcm2835_i2c_write(buf, 2));
}
/*Config Accel Range*/
void mpu6050::setFullScaleAccelRange()
{
	char buf[2]; 
	/*Write to Accel Config register*/
	buf[0] = ACCEL_CONFIG; 
	buf[1] = 0x0; //set to default 2g
	printf("ACCEL_CONFIG Register Code: %d \n", bcm2835_i2c_write(buf, 2));
}

/*Get device ID, used also for testing connection*/
uint8_t mpu6050::getDeviceID()
{
	char buf[2]; 
	char result[1]; 
	/*read Who am I register*/
	buf[0] = WHO_AM_I; 
	printf("WHO_AM_1 Register Code: %d \n", bcm2835_i2c_write(buf, 1));
	printf("WHO_AM_1 Register Code: %d \n", bcm2835_i2c_read(result, 1));
	//result[0] = (result[0] >> 1) & 0x3F;
	return result[0]; //should return 0x68 without any bit shifting 
}

/*Get 6-axis motion using references*/
void mpu6050::getMotion6(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz)
{
	char buf[1]; 
	char result[1]; 

	buf[0] = ACCEL_XOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *ax = result[0];
	buf[0] = ACCEL_XOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*ax = (*ax << 8) | ((int16_t)result[0]);

	buf[0] = ACCEL_YOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *ay = result[0];
	buf[0] = ACCEL_YOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*ay = (*ay << 8) | ((int16_t)result[0]);
    
	buf[0] = ACCEL_ZOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *az = result[0];
	buf[0] = ACCEL_ZOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*az = (*az << 8) | ((int16_t)result[0]);
	
	buf[0] = GYRO_XOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *gx = result[0];
	buf[0] = GYRO_XOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*gx = (*gx << 8) | ((int16_t)result[0]);

	buf[0] = GYRO_YOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *gy = result[0];
	buf[0] = GYRO_YOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*gy = (*gy << 8) | ((int16_t)result[0]);
    
	buf[0] = GYRO_ZOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *gz = result[0];
	buf[0] = GYRO_ZOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*gz = (*gz << 8) | ((int16_t)result[0]);
}

void mpu6050::getAccelX(int16_t *ax)
{
	char buf[1]; 
	char result[1]; 

	buf[0] = ACCEL_XOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *ax = result[0];
	buf[0] = ACCEL_XOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*ax = (*ax << 8) | ((int16_t)result[0]);

}

void mpu6050::getAccelY(int16_t *ay)
{
	char buf[1]; 
	char result[1]; 

	buf[0] = ACCEL_YOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *ay = result[0];
	buf[0] = ACCEL_YOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*ay = (*ay << 8) | ((int16_t)result[0]);
}

void mpu6050::getAccelZ(int16_t *az)
{
	char buf[1]; 
	char result[1]; 

	buf[0] = ACCEL_ZOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *az = result[0];
	buf[0] = ACCEL_ZOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*az = (*az << 8) | ((int16_t)result[0]);
}

void mpu6050::getGyroX(int16_t *gx)
{
	char buf[1]; 
	char result[1]; 

	buf[0] = GYRO_XOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *gx = result[0];
	buf[0] = GYRO_XOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*gx = (*gx << 8) | ((int16_t)result[0]);
}

void mpu6050::getGyroY(int16_t *gy)
{
	char buf[1]; 
	char result[1]; 

	buf[0] = GYRO_YOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *gy = result[0];
	buf[0] = GYRO_YOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*gy = (*gy << 8) | ((int16_t)result[0]);
}

void mpu6050::getGyroZ(int16_t *gz)
{
	char buf[1]; 
	char result[1]; 

	buf[0] = GYRO_ZOUT_H;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
    *gz = result[0];
	buf[0] = GYRO_ZOUT_L;
	bcm2835_i2c_write(buf, 1);
	bcm2835_i2c_read(result, 1);
	*gz = (*gz << 8) | ((int16_t)result[0]);
}