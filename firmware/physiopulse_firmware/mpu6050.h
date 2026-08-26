/**
 * PhysioPulse – MPU-6050 Driver
 * Lightweight I2C driver for MPU-6050 / GY-521 sensor module
 * I2C Address: 0x69 (AD0 pin = HIGH on MYOSA board)
 * 
 * Provides: Accelerometer (g), Gyroscope (°/s), Temperature (°C)
 */

#ifndef MPU6050_H
#define MPU6050_H

#include <Wire.h>

// ─── MPU-6050 Register Map ─────────────────────────────────────────────────
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_SMPLRT_DIV   0x19
#define MPU6050_CONFIG_REG   0x1A
#define MPU6050_GYRO_CONFIG  0x1B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_TEMP_OUT_H   0x41
#define MPU6050_GYRO_XOUT_H  0x43
#define MPU6050_WHO_AM_I     0x75

// ─── Scale Factors ─────────────────────────────────────────────────────────
// Accel: ±2g  → 16384 LSB/g
// Gyro:  ±250°/s → 131 LSB/(°/s)
#define ACCEL_SCALE   16384.0f
#define GYRO_SCALE    131.0f

// ─── Sensor Data Structure ─────────────────────────────────────────────────
struct MPU6050Data {
  float accelX, accelY, accelZ;   // in g
  float gyroX,  gyroY,  gyroZ;    // in °/s
  float temperature;               // in °C
  bool  valid;                     // data validity flag
};

// ─── MPU6050 Driver Class ─────────────────────────────────────────────────
class MPU6050 {
public:
  MPU6050(uint8_t addr = 0x69) : _addr(addr) {}

  /**
   * Initialize the MPU-6050 sensor.
   * @return true if sensor is detected and configured successfully
   */
  bool begin() {
    Wire.begin();
    
    // Check WHO_AM_I register (should return 0x68 regardless of I2C address)
    uint8_t whoAmI = readByte(MPU6050_WHO_AM_I);
    if (whoAmI != 0x68 && whoAmI != 0x69) {
      Serial.printf("[MPU6050] WHO_AM_I failed: 0x%02X (expected 0x68)\n", whoAmI);
      return false;
    }
    
    // Wake up from sleep mode (bit 6 = SLEEP)
    writeByte(MPU6050_PWR_MGMT_1, 0x00);
    delay(100);
    
    // Set clock source to PLL with X-axis gyro reference (best accuracy)
    writeByte(MPU6050_PWR_MGMT_1, 0x01);
    
    // Set sample rate divider: 1kHz / (1+4) = 200 Hz
    writeByte(MPU6050_SMPLRT_DIV, 0x04);
    
    // Low-pass filter: 44 Hz bandwidth
    writeByte(MPU6050_CONFIG_REG, 0x03);
    
    // Gyro: ±250°/s full scale
    writeByte(MPU6050_GYRO_CONFIG, 0x00);
    
    // Accel: ±2g full scale
    writeByte(MPU6050_ACCEL_CONFIG, 0x00);
    
    Serial.printf("[MPU6050] Initialized at I2C address 0x%02X\n", _addr);
    return true;
  }

  /**
   * Read all sensor data in one burst read (14 bytes from 0x3B)
   */
  MPU6050Data read() {
    MPU6050Data data;
    data.valid = false;
    
    Wire.beginTransmission(_addr);
    Wire.write(MPU6050_ACCEL_XOUT_H);
    if (Wire.endTransmission(false) != 0) {
      Serial.println("[MPU6050] I2C transmission error");
      return data;
    }
    
    // Request 14 bytes: 6 accel + 2 temp + 6 gyro
    uint8_t bytesRead = Wire.requestFrom(_addr, (uint8_t)14);
    if (bytesRead != 14) {
      Serial.printf("[MPU6050] Expected 14 bytes, got %d\n", bytesRead);
      return data;
    }
    
    int16_t rawAX = (Wire.read() << 8) | Wire.read();
    int16_t rawAY = (Wire.read() << 8) | Wire.read();
    int16_t rawAZ = (Wire.read() << 8) | Wire.read();
    int16_t rawT  = (Wire.read() << 8) | Wire.read();
    int16_t rawGX = (Wire.read() << 8) | Wire.read();
    int16_t rawGY = (Wire.read() << 8) | Wire.read();
    int16_t rawGZ = (Wire.read() << 8) | Wire.read();
    
    // Convert to engineering units
    data.accelX = rawAX / ACCEL_SCALE;
    data.accelY = rawAY / ACCEL_SCALE;
    data.accelZ = rawAZ / ACCEL_SCALE;
    
    data.gyroX = rawGX / GYRO_SCALE;
    data.gyroY = rawGY / GYRO_SCALE;
    data.gyroZ = rawGZ / GYRO_SCALE;
    
    // Temperature formula from MPU-6050 datasheet:
    // Temp(°C) = RawTemp / 340.0 + 36.53
    data.temperature = (rawT / 340.0f) + 36.53f;
    
    data.valid = true;
    return data;
  }

  /**
   * Calibrate gyroscope by averaging 500 samples at rest.
   * Call once during setup when sensor is stationary.
   */
  void calibrateGyro(float &offsetX, float &offsetY, float &offsetZ) {
    Serial.println("[MPU6050] Calibrating gyroscope (keep sensor still)...");
    const int SAMPLES = 500;
    double sumX = 0, sumY = 0, sumZ = 0;
    
    for (int i = 0; i < SAMPLES; i++) {
      MPU6050Data d = read();
      if (d.valid) {
        sumX += d.gyroX;
        sumY += d.gyroY;
        sumZ += d.gyroZ;
      }
      delay(2);
    }
    
    offsetX = sumX / SAMPLES;
    offsetY = sumY / SAMPLES;
    offsetZ = sumZ / SAMPLES;
    
    Serial.printf("[MPU6050] Gyro offsets: X=%.3f Y=%.3f Z=%.3f\n", offsetX, offsetY, offsetZ);
  }

  uint8_t getAddress() { return _addr; }

private:
  uint8_t _addr;
  
  void writeByte(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
  }
  
  uint8_t readByte(uint8_t reg) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(_addr, (uint8_t)1);
    return Wire.read();
  }
};

#endif // MPU6050_H
