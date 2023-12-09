/* 
 * File:   mpu6050.h
 * Author: jossi006
 *
 * Created on November 21, 2023, 10:35 AM
 */

#ifndef MPU6050_H
#define	MPU6050_H

#define ADDRESS_MPU6050_WRITE 0b11010000
#define ADDRESS_MPU6050_READ 0b11010001

// REGISTER NUMBERS
#define GYRO_CONFIG_RA 27
#define ACCEL_CONFIG_RA 28
#define FIFO_EN_RA 35
#define INT_PIN_CFG_RA 55
#define INT_ENABLE_RA 56
#define INT_STATUS_RA 58
#define GYRO_XOUT_H 67
#define GYRO_XOUT_L 68
#define GYRO_YOUT_H 69
#define GYRO_YOUT_L 70
#define GYRO_ZOUT_H 71
#define GYRO_ZOUT_L 72
#define PWR_MGMT_1_RA 107
#define FIFO_R_W 116
#define ACCEL_XOUT_H 59
#define ACCEL_XOUT_L 60
#define ACCEL_YOUT_H 61
#define ACCEL_YOUT_L 62
#define ACCEL_ZOUT_H 63
#define ACCEL_ZOUT_l 64

// DATA VALUES
#define GYRO_CONFIG_DATA 0b00011000 // 250 deg/s
#define ACCEL_CONFIG_DATA 0b00001000 // 4g
#define FIFO_EN_DATA 0b00000000 //0
#define INT_EN_DATA 0b00000000 //0
#define PWR_MGMT_1_DATA 0b00001001 //9
#define GYRO_SELF_TEST (0b11100000 & GYRO_CONFIG_DATA)

// Degree calculations
#define MPU_6050_UPDATE_TIME_MS 25
#define NUM_CAL_SAMPLES 512
#define SHIFT_AMOUNT 22 // 2^22 = 4,194,304
#define SHIFT_MASK ((1 << SHIFT_AMOUNT) - 1) // 4,194,303, all the fraction is set
#define FRACTIONAL_BITS 22
#define INTEGER_BITS 9

// Function definitions
void write_register(uint8_t reg, uint8_t data);
int16_t read_register(uint8_t reg);
void setup_mpu6050();
int16_t get_gyroX();
int16_t get_gyroY();
int16_t get_gyroZ();
void update_degrees(void);
void calibrate_gyro(void);
float get_degX(void);
float get_degY(void);
float get_degZ(void);

#endif	/* MPU6050_H */
