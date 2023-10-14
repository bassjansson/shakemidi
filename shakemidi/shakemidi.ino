#include <Arduino.h>
#include "MPU9250.h"
#include "eeprom_utils.h"

// #define CALIBRATE_IMU

MPU9250 mpu;

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

    if (!mpu.setup(0x68))
    { // change to your own address
        while (1)
        {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }

#if defined(ESP_PLATFORM) || defined(ESP8266)
    EEPROM.begin(0x80);
#endif

    delay(5000);

#ifdef CALIBRATE_IMU
    // calibrate anytime you want to
    Serial.println("Accel Gyro calibration will start in 5sec.");
    Serial.println("Please leave the device still on the flat plane.");
    mpu.verbose(true);
    delay(5000);
    mpu.calibrateAccelGyro();

    Serial.println("Mag calibration will start in 5sec.");
    Serial.println("Please Wave device in a figure eight until done.");
    delay(5000);
    mpu.calibrateMag();

    print_calibration();
    mpu.verbose(false);

    // save to eeprom
    saveCalibration();
#endif

    // load from eeprom
    loadCalibration();
}

void loop()
{
    if (mpu.update())
    {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 25)
        {
            print_roll_pitch_yaw();
            prev_ms = millis();
        }
    }
}

void print_roll_pitch_yaw()
{
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);
}

#ifdef CALIBRATE_IMU
void print_calibration()
{
    Serial.println("< calibration parameters >");
    Serial.println("accel bias [g]: ");
    Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.println();
    Serial.println("gyro bias [deg/s]: ");
    Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.println();
    Serial.println("mag bias [mG]: ");
    Serial.print(mpu.getMagBiasX());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasY());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasZ());
    Serial.println();
    Serial.println("mag scale []: ");
    Serial.print(mpu.getMagScaleX());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());
    Serial.println();
}
#endif

/*
#include <Arduino.h>
#include <BLEMidi.h>

#define PUSH_BUTTON_1_PIN 34
#define PUSH_BUTTON_2_PIN 36
#define PUSH_BUTTON_3_PIN 39

void setup()
{
    pinMode(PUSH_BUTTON_1_PIN, INPUT);
    pinMode(PUSH_BUTTON_2_PIN, INPUT);
    pinMode(PUSH_BUTTON_3_PIN, INPUT);

    Serial.begin(115200);

    Serial.println("Initializing bluetooth");
    BLEMidiServer.begin("I love you!");

    Serial.println("Waiting for connections...");
    // BLEMidiServer.enableDebugging(); // Uncomment if you want to see some debugging output from the library
}

void sendNote(int& value, int pin, int note)
{
    int newVal = digitalRead(pin);

    if (value != newVal)
    {
        value = newVal;
        value ?
            BLEMidiServer.noteOff(0, note, 0) :
            BLEMidiServer.noteOn(0, note, 127);

    }
}

void loop()
{
    static int pushButton1 = 0;
    static int pushButton2 = 0;
    static int pushButton3 = 0;

    if (BLEMidiServer.isConnected())
    {
        sendNote(pushButton1, PUSH_BUTTON_1_PIN, 36);
        sendNote(pushButton2, PUSH_BUTTON_2_PIN, 38);
        sendNote(pushButton3, PUSH_BUTTON_3_PIN, 42);

        delay(10);
    }
    else
    {
        delay(100);
    }
}
*/
