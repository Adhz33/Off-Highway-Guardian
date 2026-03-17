/*
 * Simplified MPU6050 library for Off-Highway Guardian
 * Based on I2Cdev library
 */

#ifndef MPU6050_H
#define MPU6050_H

#include <Wire.h>

#define MPU6050_ADDRESS 0x68

// Register addresses
#define MPU6050_RA_ACCEL_XOUT_H 0x3B
#define MPU6050_RA_GYRO_XOUT_H 0x43
#define MPU6050_RA_PWR_MGMT_1 0x6B
#define MPU6050_RA_ACCEL_CONFIG 0x1C
#define MPU6050_RA_GYRO_CONFIG 0x1B
#define MPU6050_RA_WHO_AM_I 0x75

// Accel scale ranges
#define MPU6050_ACCEL_FS_2 0x00
#define MPU6050_ACCEL_FS_4 0x01
#define MPU6050_ACCEL_FS_8 0x02
#define MPU6050_ACCEL_FS_16 0x03

// Gyro scale ranges
#define MPU6050_GYRO_FS_250 0x00
#define MPU6050_GYRO_FS_500 0x01
#define MPU6050_GYRO_FS_1000 0x02
#define MPU6050_GYRO_FS_2000 0x03

class MPU6050 {
public:
    MPU6050() : address(MPU6050_ADDRESS) {}
    
    void initialize() {
        // Wake up MPU6050
        writeByte(MPU6050_RA_PWR_MGMT_1, 0x00);
        delay(100);
    }
    
    bool testConnection() {
        uint8_t whoami = readByte(MPU6050_RA_WHO_AM_I);
        return (whoami == 0x68 || whoami == 0x72);
    }
    
    void setFullScaleAccelRange(uint8_t range) {
        writeBits(MPU6050_RA_ACCEL_CONFIG, 4, 2, range);
    }
    
    void setFullScaleGyroRange(uint8_t range) {
        writeBits(MPU6050_RA_GYRO_CONFIG, 4, 2, range);
    }
    
    void getMotion6(int16_t* ax, int16_t* ay, int16_t* az, 
                    int16_t* gx, int16_t* gy, int16_t* gz) {
        uint8_t buffer[14];
        readBytes(MPU6050_RA_ACCEL_XOUT_H, 14, buffer);
        
        *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
        *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
        *az = (((int16_t)buffer[4]) << 8) | buffer[5];
        *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
        *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
        *gz = (((int16_t)buffer[12]) << 8) | buffer[13];
    }

private:
    uint8_t address;
    
    void writeByte(uint8_t reg, uint8_t data) {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.write(data);
        Wire.endTransmission();
    }
    
    uint8_t readByte(uint8_t reg) {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(address, (uint8_t)1);
        return Wire.read();
    }
    
    void readBytes(uint8_t reg, uint8_t count, uint8_t* dest) {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(address, count);
        for (uint8_t i = 0; i < count; i++) {
            dest[i] = Wire.read();
        }
    }
    
    void writeBits(uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t data) {
        uint8_t b = readByte(reg);
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1);
        data &= mask;
        b &= ~mask;
        b |= data;
        writeByte(reg, b);
    }
};

#endif // MPU6050_H
