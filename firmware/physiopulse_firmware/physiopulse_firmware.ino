/**
 * ╔════════════════════════════════════════════════════════════════════════════╗
 * ║     PhysioTracker – Ultra-Lightweight BLE IMU Transmitter Firmware         ║
 * ║     MYOSA ESP32-WROVER + MPU-6050 (100% Standalone Single File)            ║
 * ╚════════════════════════════════════════════════════════════════════════════╝
 *
 * Requirements:
 *   - ESP32 Board Package installed in Arduino IDE
 *   - ZERO external third-party library installations required!
 *
 * Architecture:
 *   - Pure Sensor Streamer: Samples MPU-6050 at 50Hz.
 *   - Streams raw IMU packets over BLE Notifications at 20-50Hz:
 *     {"ax":0.123,"ay":-0.981,"az":0.045,"gx":1.2,"gy":-0.4,"gz":0.1,"tp":36.5}
 *   - All vector math, 2-step calibration, baseline recording, and rep detection
 *     are executed by the software platform (Web App).
 */

#include <Arduino.h>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ─── Configuration ─────────────────────────────────────────────────────────
#define DEVICE_BLE_NAME           "PhysioTracker"
#define STATUS_LED                2
#define SENSOR_INTERVAL_MS        20   // 50Hz sensor sample rate

// BLE GATT UUIDs
#define BLE_SERVICE_UUID          "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLE_CHAR_TELEMETRY_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26a1"
#define BLE_CHAR_COMMAND_UUID     "beb5483e-36e1-4688-b7f5-ea07361b26a2"

// ─── Embedded Self-Contained MPU-6050 Driver ───────────────────────────────
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_SMPLRT_DIV   0x19
#define MPU6050_CONFIG_REG   0x1A
#define MPU6050_GYRO_CONFIG  0x1B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_WHO_AM_I     0x75

#define ACCEL_SCALE   16384.0f   // ±2g sensitivity (16384 LSB/g)
#define GYRO_SCALE    131.0f     // ±250°/s sensitivity (131 LSB/°/s)

struct MPU6050Data {
  float accelX, accelY, accelZ;
  float gyroX,  gyroY,  gyroZ;
  float temperature;
  bool  valid;
};

class MPU6050 {
public:
  MPU6050(uint8_t addr = 0x69) : _addr(addr) {}

  bool begin() {
    Wire.begin();
    Wire.setClock(400000); // 400kHz Fast I2C
    uint8_t whoAmI = readByte(MPU6050_WHO_AM_I);
    if (whoAmI != 0x68 && whoAmI != 0x69) {
      return false;
    }
    writeByte(MPU6050_PWR_MGMT_1, 0x00); // Wake up MPU-6050
    delay(50);
    writeByte(MPU6050_PWR_MGMT_1, 0x01); // Clock source = Gyro X
    writeByte(MPU6050_SMPLRT_DIV, 0x04); // 200Hz internal sample rate
    writeByte(MPU6050_CONFIG_REG, 0x03); // DLPF ~44Hz
    writeByte(MPU6050_GYRO_CONFIG, 0x00); // ±250°/s
    writeByte(MPU6050_ACCEL_CONFIG, 0x00); // ±2g
    return true;
  }

  MPU6050Data read() {
    MPU6050Data data;
    data.valid = false;

    Wire.beginTransmission(_addr);
    Wire.write(MPU6050_ACCEL_XOUT_H);
    if (Wire.endTransmission(false) != 0) return data;

    uint8_t bytesRead = Wire.requestFrom(_addr, (uint8_t)14);
    if (bytesRead != 14) return data;

    int16_t rawAX = (Wire.read() << 8) | Wire.read();
    int16_t rawAY = (Wire.read() << 8) | Wire.read();
    int16_t rawAZ = (Wire.read() << 8) | Wire.read();
    int16_t rawT  = (Wire.read() << 8) | Wire.read();
    int16_t rawGX = (Wire.read() << 8) | Wire.read();
    int16_t rawGY = (Wire.read() << 8) | Wire.read();
    int16_t rawGZ = (Wire.read() << 8) | Wire.read();

    data.accelX = rawAX / ACCEL_SCALE;
    data.accelY = rawAY / ACCEL_SCALE;
    data.accelZ = rawAZ / ACCEL_SCALE;
    data.gyroX  = rawGX / GYRO_SCALE;
    data.gyroY  = rawGY / GYRO_SCALE;
    data.gyroZ  = rawGZ / GYRO_SCALE;
    data.temperature = (rawT / 340.0f) + 36.53f;
    data.valid = true;
    return data;
  }

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

// ─── Global State & Handles ────────────────────────────────────────────────
MPU6050 mpu(0x69);
BLEServer *pBLEServer = nullptr;
BLECharacteristic *pCharTelemetry = nullptr;
BLECharacteristic *pCharCommand = nullptr;
bool bleDeviceConnected = false;
unsigned long lastSampleMillis = 0;

// ─── BLE Callbacks ─────────────────────────────────────────────────────────
class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    bleDeviceConnected = true;
    digitalWrite(STATUS_LED, HIGH);
    Serial.println("[BLE] Web client connected!");
  }
  void onDisconnect(BLEServer* pServer) {
    bleDeviceConnected = false;
    digitalWrite(STATUS_LED, LOW);
    Serial.println("[BLE] Client disconnected. Restarting advertising...");
    pServer->startAdvertising();
  }
};

class CommandCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String rxValue = pCharacteristic->getValue().c_str();
    rxValue.trim();
    if (rxValue.length() > 0) {
      Serial.printf("[BLE RX CMD] %s\n", rxValue.c_str());
    }
  }
};

void setupBLE() {
  BLEDevice::init(DEVICE_BLE_NAME);
  pBLEServer = BLEDevice::createServer();
  pBLEServer->setCallbacks(new ServerCallbacks());

  BLEService *pService = pBLEServer->createService(BLE_SERVICE_UUID);

  // Telemetry Characteristic (Notify)
  pCharTelemetry = pService->createCharacteristic(
    BLE_CHAR_TELEMETRY_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharTelemetry->addDescriptor(new BLE2902());

  // Command Characteristic (Write)
  pCharCommand = pService->createCharacteristic(
    BLE_CHAR_COMMAND_UUID,
    BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
  );
  pCharCommand->setCallbacks(new CommandCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  BLEDevice::startAdvertising();

  Serial.printf("[BLE] Broadcasting as '%s'\n", DEVICE_BLE_NAME);
}

// ─── Arduino Setup ─────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n╔═══════════════════════════════════════════════════════╗");
  Serial.println("║  PhysioTracker – High-Speed BLE IMU Transmitter       ║");
  Serial.println("║  Raw 50Hz Sensor Stream -> Software Processing Engine ║");
  Serial.println("╚═══════════════════════════════════════════════════════╝");

  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);

  // Try 0x69 first, fallback to 0x68 if AD0 is grounded
  if (!mpu.begin()) {
    MPU6050 mpuAlt(0x68);
    if (mpuAlt.begin()) {
      mpu = mpuAlt;
      Serial.println("[MPU] MPU-6050 initialized at address 0x68");
    } else {
      Serial.println("[MPU] ERROR: MPU-6050 not found on I2C bus!");
    }
  } else {
    Serial.println("[MPU] MPU-6050 initialized at address 0x69");
  }

  setupBLE();
  Serial.println("[READY] Ready for Web Bluetooth connection.");
}

// ─── Main Loop ─────────────────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();
  if (now - lastSampleMillis >= SENSOR_INTERVAL_MS) {
    lastSampleMillis = now;

    MPU6050Data data = mpu.read();
    if (data.valid && bleDeviceConnected && pCharTelemetry != nullptr) {
      char buf[160];
      snprintf(buf, sizeof(buf),
               "{\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f,\"gx\":%.2f,\"gy\":%.2f,\"gz\":%.2f,\"tp\":%.1f}",
               data.accelX, data.accelY, data.accelZ,
               data.gyroX,  data.gyroY,  data.gyroZ,
               data.temperature);

      pCharTelemetry->setValue(buf);
      pCharTelemetry->notify();
    }
  }
}
