#pragma once

#include "../../Driver/Twi/TwiInterface.h"
#include "../../Driver/Tick/TickInterface.h"
#include "Mpu9150Interface.h"
#include "Ahrs.h"

namespace Component
{
    namespace Imu
    {
        using namespace Driver;
        using namespace Misc::Maths;

        class Mpu9150 : public Mpu9150Interface {
        public:
            static constexpr uint8_t MPU9150_I2C_ADDRESS = 0x69U;
            static constexpr uint8_t AK8963_I2C_ADDRESS = 0x0CU;

            struct ERegister {
                static constexpr uint8_t SELF_TEST_X = 0x0D;
                static constexpr uint8_t SELF_TEST_Y = 0x0E;
                static constexpr uint8_t SELF_TEST_Z = 0x0F;
                static constexpr uint8_t SELF_TEST_A = 0x10;
                static constexpr uint8_t SMPLRT_DIV = 0x19;
                static constexpr uint8_t CONFIG = 0x1A;
                static constexpr uint8_t GYRO_CONFIG = 0x1B;
                static constexpr uint8_t ACCEL_CONFIG = 0x1C;
                static constexpr uint8_t FF_THR = 0x1D;
                static constexpr uint8_t FF_DUR = 0x1E;
                static constexpr uint8_t MOT_THR = 0x1F;
                static constexpr uint8_t MOT_DUR = 0x20;
                static constexpr uint8_t ZRMOT_THR = 0x21;
                static constexpr uint8_t ZRMOT_DUR = 0x22;
                static constexpr uint8_t FIFO_EN = 0x23;
                static constexpr uint8_t I2C_MST_CTRL = 0x24;
                static constexpr uint8_t I2C_SLV0_ADDR = 0x25;
                static constexpr uint8_t I2C_SLV0_REG = 0x26;
                static constexpr uint8_t I2C_SLV0_CTRL = 0x27;
                static constexpr uint8_t I2C_SLV1_ADDR = 0x28;
                static constexpr uint8_t I2C_SLV1_REG = 0x29;
                static constexpr uint8_t I2C_SLV1_CTRL = 0x2A;
                static constexpr uint8_t I2C_SLV2_ADDR = 0x2B;
                static constexpr uint8_t I2C_SLV2_REG = 0x2C;
                static constexpr uint8_t I2C_SLV2_CTRL = 0x2D;
                static constexpr uint8_t I2C_SLV3_ADDR = 0x2E;
                static constexpr uint8_t I2C_SLV3_REG = 0x2F;
                static constexpr uint8_t I2C_SLV3_CTRL = 0x30;
                static constexpr uint8_t I2C_SLV4_ADDR = 0x31;
                static constexpr uint8_t I2C_SLV4_REG = 0x32;
                static constexpr uint8_t I2C_SLV4_DO = 0x33;
                static constexpr uint8_t I2C_SLV4_CTRL = 0x34;
                static constexpr uint8_t I2C_SLV4_DI = 0x35;
                static constexpr uint8_t I2C_MST_STATUS = 0x36;
                static constexpr uint8_t INT_PIN_CFG = 0x37;
                static constexpr uint8_t INT_ENABLE = 0x38;
                static constexpr uint8_t INT_STATUS = 0x3A;
                static constexpr uint8_t ACCEL_XOUT_H = 0x3B;
                static constexpr uint8_t ACCEL_XOUT_L = 0x3C;
                static constexpr uint8_t ACCEL_YOUT_H = 0x3D;
                static constexpr uint8_t ACCEL_YOUT_L = 0x3E;
                static constexpr uint8_t ACCEL_ZOUT_H = 0x3F;
                static constexpr uint8_t ACCEL_ZOUT_L = 0x40;
                static constexpr uint8_t TEMP_OUT_H = 0x41;
                static constexpr uint8_t TEMP_OUT_L = 0x42;
                static constexpr uint8_t GYRO_XOUT_H = 0x43;
                static constexpr uint8_t GYRO_XOUT_L = 0x44;
                static constexpr uint8_t GYRO_YOUT_H = 0x45;
                static constexpr uint8_t GYRO_YOUT_L = 0x46;
                static constexpr uint8_t GYRO_ZOUT_H = 0x47;
                static constexpr uint8_t GYRO_ZOUT_L = 0x48;
                static constexpr uint8_t EXT_SENS_DATA_00 = 0x49;
                static constexpr uint8_t EXT_SENS_DATA_01 = 0x4A;
                static constexpr uint8_t EXT_SENS_DATA_02 = 0x4B;
                static constexpr uint8_t EXT_SENS_DATA_03 = 0x4C;
                static constexpr uint8_t EXT_SENS_DATA_04 = 0x4D;
                static constexpr uint8_t EXT_SENS_DATA_05 = 0x4E;
                static constexpr uint8_t EXT_SENS_DATA_06 = 0x4F;
                static constexpr uint8_t EXT_SENS_DATA_07 = 0x50;
                static constexpr uint8_t EXT_SENS_DATA_08 = 0x51;
                static constexpr uint8_t EXT_SENS_DATA_09 = 0x52;
                static constexpr uint8_t EXT_SENS_DATA_10 = 0x53;
                static constexpr uint8_t EXT_SENS_DATA_11 = 0x54;
                static constexpr uint8_t EXT_SENS_DATA_12 = 0x55;
                static constexpr uint8_t EXT_SENS_DATA_13 = 0x56;
                static constexpr uint8_t EXT_SENS_DATA_14 = 0x57;
                static constexpr uint8_t EXT_SENS_DATA_15 = 0x58;
                static constexpr uint8_t EXT_SENS_DATA_16 = 0x59;
                static constexpr uint8_t EXT_SENS_DATA_17 = 0x5A;
                static constexpr uint8_t EXT_SENS_DATA_18 = 0x5B;
                static constexpr uint8_t EXT_SENS_DATA_19 = 0x5C;
                static constexpr uint8_t EXT_SENS_DATA_20 = 0x5D;
                static constexpr uint8_t EXT_SENS_DATA_21 = 0x5E;
                static constexpr uint8_t EXT_SENS_DATA_22 = 0x5F;
                static constexpr uint8_t EXT_SENS_DATA_23 = 0x60;
                static constexpr uint8_t MOT_DETECT_STATUS = 0x61;
                static constexpr uint8_t I2C_SLV0_DO = 0x63;
                static constexpr uint8_t I2C_SLV1_DO = 0x64;
                static constexpr uint8_t I2C_SLV2_DO = 0x65;
                static constexpr uint8_t I2C_SLV3_DO = 0x66;
                static constexpr uint8_t I2C_MST_DELAY_CTRL = 0x67;
                static constexpr uint8_t SIGNAL_PATH_RESET = 0x68;
                static constexpr uint8_t MOT_DETECT_CTRL = 0x69;
                static constexpr uint8_t USER_CTRL = 0x6A;
                static constexpr uint8_t PWR_MGMT_1 = 0x6B;
                static constexpr uint8_t PWR_MGMT_2 = 0x6C;
                static constexpr uint8_t FIFO_COUNTH = 0x72;
                static constexpr uint8_t FIFO_COUNTL = 0x73;
                static constexpr uint8_t FIFO_R_W = 0x74;
                static constexpr uint8_t WHO_AM_I = 0x75;
                static constexpr uint8_t PWR1_CLKSEL_BIT = 2;
                static constexpr uint8_t PWR1_SLEEP_BIT = 6;
            };


            Mpu9150(Twi::TwiInterface &i2c, Tick::TickInterface &tick, const uint8_t address = MPU9150_I2C_ADDRESS);

            ~Mpu9150() = default;

            Core::CoreStatus Initialize(void);

            void Update(const uint64_t currentTime);

            inline virtual Vector3 ReadAcc(void) final override {
                return (this->mAccRaw);
            }

            inline virtual Vector3 ReadGyr(void) final override {
                return (this->mGyrRaw);
            }

            inline virtual Vector3 ReadMag(void) final override {
                return (this->mMagRaw);
            }

            inline virtual int16_t ReadTemp(void) final override {
                return (this->mTmp);
            }

            inline virtual Position3D ReadYawPitchRoll(void) final override {
                return (this->mYawPitchRoll);
            }

            inline virtual void StartCalibrationMag(const bool enable) {
                this->mStartMagCalib = enable;
				if(enable == false)
				{
					this->mMagOffset.x = (this->mMagCalibMax.x - this->mMagCalibMin.x) / 2U;
					this->mMagOffset.y = (this->mMagCalibMax.y - this->mMagCalibMin.y) / 2U;
					this->mMagOffset.z = (this->mMagCalibMax.z - this->mMagCalibMin.z) / 2U;
				}
            }

            inline virtual Vector3F ReadCalibrationMag(const bool min) {
                if (min == true) {
                    return this->mMagCalibMin;
                }
                return this->mMagCalibMax;
            }

        private:
            Vector3 UpdateAcc(void);

            Vector3 UpdateGyr(void);

            void AdjustingMag(void);

            Vector3 UpdateMag(void);

            int16_t UpdateTemp(void);

            bool IsDataReady(void) const;

            struct ERegisterAccel {
                static constexpr uint8_t ACCEL_FS_2 = 0x00;
                static constexpr uint8_t ACCEL_FS_4 = 0x01;
                static constexpr uint8_t ACCEL_FS_8 = 0x02;
                static constexpr uint8_t ACCEL_FS_16 = 0x03;

                static constexpr uint8_t DLPF_BW_256 = 0x00;
                static constexpr uint8_t DLPF_BW_188 = 0x01;
                static constexpr uint8_t DLPF_BW_98 = 0x02;
                static constexpr uint8_t DLPF_BW_42 = 0x03;
                static constexpr uint8_t DLPF_BW_20 = 0x04;
                static constexpr uint8_t DLPF_BW_10 = 0x05;
                static constexpr uint8_t DLPF_BW_5 = 0x06;

                static constexpr uint16_t ACCEL_1G_2G = 0x3FFF;
                static constexpr uint16_t ACCEL_1G_4G = 0x1FFF;
                static constexpr uint16_t ACCEL_1G_8G = 0x0FFF;
                static constexpr uint16_t ACCEL_1G_16G = 0x07FF;
            };

            struct ERegisterMag {
                static constexpr uint8_t WHO_AM_I = 0x00;
                static constexpr uint8_t INFO = 0x01;
                static constexpr uint8_t ST1 = 0x02;
                static constexpr uint8_t XOUT_L = 0x03;
                static constexpr uint8_t XOUT_H = 0x04;
                static constexpr uint8_t YOUT_L = 0x05;
                static constexpr uint8_t YOUT_H = 0x06;
                static constexpr uint8_t ZOUT_L = 0x07;
                static constexpr uint8_t ZOUT_H = 0x08;
                static constexpr uint8_t ST2 = 0x09;
                static constexpr uint8_t CNTL = 0x0A;
                static constexpr uint8_t ASTC = 0x0C;
                static constexpr uint8_t I2CDIS = 0x0F;
                static constexpr uint8_t ASAX = 0x10;
                static constexpr uint8_t ASAY = 0x11;
                static constexpr uint8_t ASAZ = 0x12;
            };

            struct ERegisterGyro {
                static constexpr uint8_t CLOCK_PLL_XGYRO = 0x01;
                static constexpr uint8_t GYRO_FS_250 = 0x00;
                static constexpr uint8_t GYRO_FS_500 = 0x01;
                static constexpr uint8_t GYRO_FS_1000 = 0x02;
                static constexpr uint8_t GYRO_FS_2000 = 0x03;

                static constexpr uint8_t SELF_TEST_X_GYRO = 0x00;
                static constexpr uint8_t SELF_TEST_Y_GYRO = 0x01;
                static constexpr uint8_t SELF_TEST_Z_GYRO = 0x02;
            };

            Twi::TwiInterface &mI2c;
            Tick::TickInterface &mTick;
            const uint8_t mAddress;
            const uint8_t mAddressMag;
            Vector3 mAccOffset;
            Vector3 mGyrOffset;
            Vector3F mMagOffset;
            Vector3F mMagBias;
            bool mStartMagCalib;
            Vector3F mMagCalibMin;
            Vector3F mMagCalibMax;
            Vector3 mAccRaw;
            Vector3 mGyrRaw;
            Vector3 mMagRaw;
            Vector3F mAcc;
            Vector3F mGyr;
            Vector3F mMag;
            uint8_t mTmp;
            uint64_t mLastLoopTime;
            Ahrs mAhrs;
            Position3D mYawPitchRoll;
        };
    }
}
