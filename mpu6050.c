#include <xc.h>
#include "mpu6050.h"
#include <stdio.h>

// GLOBAL VARIABLES
/*
 * Fixed Point Representation:
 * Want to represent anywhere from -180 to +180 degrees
 * We will use 32 bits. 2's complement with 10 for the integer and 22 for the fraction
 *  Integer          Fraction
 * 0000 0000 00 . 00 0000 0000 0000
 * Add: add number shifted
 * multiply: can bit shift or just regular multiply
 * divide: can bit shift or just regular divide
 */
float degX, degY, degZ;
volatile int16_t velX_cal, velY_cal, velZ_cal;

/* 
 * Writes a 8 bit value to register given the register address
 * and the data to pass into it.
 * Parameters: uint8_t register address, uint8_t data
 * Return: void
 */
void write_register(uint8_t reg, uint8_t data) {
    /* Can check I2CxSTATbits.P == 1 to see if the I2C bus is free\
    *  Do we need to wait for the acknowledge bit after sending data??
    *  Start bit */
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN);
    
    // Slave Address byte
    I2C2TRN = ADDRESS_MPU6050_WRITE;
    while(I2C2STATbits.TRSTAT);
    
    // Register Address byte
    I2C2TRN = reg;
    while (I2C2STATbits.TRSTAT);
    
    // Data Byte
    I2C2TRN = data;
    while (I2C2STATbits.TRSTAT);
    
    // Stop bit
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN);
    
    IFS3bits.MI2C2IF = 0;
}

/* 
 * Reads a 16 bit value from two registers given the starting address
 * It reads the first register starting at the given address, 
 * and then the next register. Returning the first data as the high byte,
 * and the second as the low byte. Used ideally for reading the 
 * gyroscope and accelerometer values
 * Parameters: uint8_t starting register address
 * Return: uint16_t
 */
int16_t read_register(uint8_t reg) {
    int16_t data = 0;
    // Can check I2CxSTATbits.P == 1 to see if the I2C bus is free
    // Start bit
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN);
    
    // Slave Address byte write
    I2C2TRN = ADDRESS_MPU6050_WRITE;
    while(I2C2STATbits.TRSTAT);
    
    // Register Address byte
    I2C2TRN = reg;
    while(I2C2STATbits.TRSTAT);
    
    // Repeated Start bit
    I2C2CONbits.RSEN = 1;
    while (I2C2CONbits.RSEN);
    
    // Slave Address byte read
    I2C2TRN = ADDRESS_MPU6050_READ;
    while (I2C2STATbits.TRSTAT);
    
    // Receive data upper byte
    I2C2CONbits.RCEN = 1;
    while (I2C2CONbits.RCEN);
    data = ((int16_t)I2C2RCV) << 8;
    
    // ACK signal
    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1;
    while (I2C2CONbits.ACKEN);
    
    // Receive data lower byte
    I2C2CONbits.RCEN = 1;
    while (I2C2CONbits.RCEN);
    data |= (int16_t)I2C2RCV;
    
    // NACK signal
    I2C2CONbits.ACKDT = 1;
    I2C2CONbits.ACKEN = 1;
    while (I2C2CONbits.ACKEN);
    
    // Stop bit
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN);
    
    IFS3bits.MI2C2IF = 0;
    return data;
}

/*
 * Sets up the MPU6050. This puts the device out of sleep mode, 
 * configures the gyroscope to 2000 deg/s, the accelerometer to 
 * 4g's, disables the FIFO buffer, and disables interrupts.
 * Parameters: none
 * Return: void
 */
void setup_mpu6050() {
    write_register(PWR_MGMT_1_RA, PWR_MGMT_1_DATA);
    write_register(GYRO_CONFIG_RA, GYRO_CONFIG_DATA);
    write_register(ACCEL_CONFIG_RA, ACCEL_CONFIG_DATA);
    write_register(FIFO_EN_RA, FIFO_EN_DATA);
    write_register(INT_ENABLE_RA, INT_EN_DATA);
//    write_register(GYRO_CONFIG_RA, GYRO_SELF_TEST);
}

/*
 * Gets the current X value of the gyroscope as a 16 bit signed int
 */
int16_t get_gyroX() {
    return read_register(GYRO_XOUT_H);
}

/*
 * Gets the current Y value of the gyroscope as a 16 bit signed int
 */
int16_t get_gyroY() {
    return read_register(GYRO_YOUT_H);
}

/*
 * Gets the current Z value of the gyroscope as a 16 bit signed int
 */
int16_t get_gyroZ() {
    return read_register(GYRO_ZOUT_H);
}

void calibrate_gyro(void) {
    // Average 500 samples to calibrate the device
    long xVel = 0, yVel = 0, zVel = 0;
    for (int i = 0; i < NUM_CAL_SAMPLES; i++) {
        xVel += get_gyroX();
        yVel += get_gyroY();
        zVel += get_gyroZ();
        for (long i = 0; i < 2; i++) {
        asm("repeat #15993");
        asm("nop");
    }
    }
    velX_cal = xVel / NUM_CAL_SAMPLES;
    velY_cal = yVel / NUM_CAL_SAMPLES;
    velZ_cal = zVel / NUM_CAL_SAMPLES;
    degX = 0;
    degY = 0;
    degZ = 0;
}

void update_degrees(void) {
    degX += (MPU_6050_UPDATE_TIME_MS/1000.0)*((get_gyroX()-velX_cal)/16.38);
    degY += (MPU_6050_UPDATE_TIME_MS/1000.0)*((get_gyroY()-velY_cal)/16.38);
    degZ += (MPU_6050_UPDATE_TIME_MS/1000.0)*((get_gyroZ()-velZ_cal)/16.38);
    
}

float get_degX() {
    return degX;
}

float get_degY() {
    return degY;
}

float get_degZ() {
    return degZ;
}