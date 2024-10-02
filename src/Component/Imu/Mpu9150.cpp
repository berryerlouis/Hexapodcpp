#include "Mpu9150.h"

namespace Component
{
    namespace Imu
    {
#define NB_SAMPLES    20

        Mpu9150::Mpu9150(Twi::TwiInterface &i2c, Tick::TickInterface &tick, const uint8_t address)
            : mI2c(i2c)
              , mTick(tick)
              , mAddress(address)
              , mAddressMag(AK8963_I2C_ADDRESS)
              , mAccOffset{0, 0, 0}
              , mGyrOffset{0, 0, 0}
              , mMagOffset{0, 0, 0}
              , mAcc{0, 0, 0}
              , mGyr{0, 0, 0}
              , mMag{0, 0, 0}
              , mMagRate(0U)
              , mTmp(0U)
              , mLastLoopTime(0U)
              , mAhrs()
              , mYawPitchRoll{0, 0, 0} {
        }

        Core::CoreStatus Mpu9150::Initialize(void) {
            uint8_t whoAmI = 0x00U;
            uint8_t reg = 0x00U;
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

            // check device
            this->mI2c.ReadRegister(this->mAddress, (uint8_t) ERegister::WHO_AM_I, whoAmI);

            if (whoAmI == this->mAddress - 1) {
                this->mI2c.ReadRegister(this->mAddressMag, (uint8_t) ERegisterMag::WHO_AM_I, whoAmI);
                // if(whoAmI == 0x48)
                //{
                this->mI2c.WriteRegister(this->mAddressMag, (uint8_t) ERegisterMag::CNTL, 0x00);
                this->mI2c.WriteRegister(this->mAddressMag, (uint8_t) ERegisterMag::CNTL, 0x0F);
                this->mI2c.WriteRegister(this->mAddressMag, (uint8_t) ERegisterMag::CNTL, 0x00);
                // }
                // setSleepEnabled
                this->mI2c.ReadRegister(this->mAddress, (uint8_t) ERegister::PWR_MGMT_1, reg);
                reg &= ~(1 << ERegister::PWR1_SLEEP_BIT);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::PWR_MGMT_1, reg);

                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_MST_CTRL, 0x40);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV0_ADDR, 0x8C);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV0_REG, 0x02);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV0_CTRL, 0x88);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV1_ADDR, 0x0C);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV1_REG, 0x0A);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV1_CTRL, 0x81);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV1_DO, 0x01);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_MST_DELAY_CTRL, 0x03);

                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV4_CTRL, 0x04);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV1_DO, 0x00);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::USER_CTRL, 0x00);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV1_DO, 0x01);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::USER_CTRL, 0x20);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::I2C_SLV4_CTRL, 0x13);

                // setClockSource
                this->mI2c.ReadRegister(this->mAddress, (uint8_t) ERegister::PWR_MGMT_1, reg);
                reg |= ERegisterGyro::CLOCK_PLL_XGYRO;
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::PWR_MGMT_1, reg);

                // setFullScaleGyroRange
                this->mI2c.ReadRegister(this->mAddress, (uint8_t) ERegister::GYRO_CONFIG, reg);
                reg |= (ERegisterGyro::GYRO_FS_250 << 3U);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::GYRO_CONFIG, reg);

                // setFullScaleAccelRange
                this->mI2c.ReadRegister(this->mAddress, (uint8_t) ERegister::ACCEL_CONFIG, reg);
                reg |= (ERegisterAccel::ACCEL_FS_2 << 3U);
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::ACCEL_CONFIG, reg);

                // setBandWidthLowPassFilter
                this->mI2c.ReadRegister(this->mAddress, (uint8_t) ERegister::CONFIG, reg);
                reg |= ERegisterAccel::DLPF_BW_20;
                this->mI2c.WriteRegister(this->mAddress, (uint8_t) ERegister::CONFIG, reg);
                success = Core::CoreStatus::CORE_OK;
            }
            return (success);
        }

        void Mpu9150::Update(const uint64_t currentTime) {
            (void) currentTime;
            this->UpdateTemp();
            Vector3F acc = this->UpdateAcc();
            Vector3F gyr = this->UpdateGyr();
            Vector3F mag = {0, 0, 0};
            if (this->mMagRate++ == 10U) {
                this->mMagRate = 0U;
                mag = this->UpdateMag();
            }

            const uint64_t now = this->mTick.GetUs();
            const uint64_t deltaTime = ((now - this->mLastLoopTime) / 1000000.0F);
            this->mLastLoopTime = now;

            this->mAhrs.MadgwickQuaternionUpdate(acc, gyr, mag, deltaTime);
            this->mAhrs.GetYawPitchRoll(this->mYawPitchRoll);
        }

        Vector3F Mpu9150::UpdateAcc(void) {
            Vector3 cmp;
            if (this->mI2c.ReadRegisters(this->mAddress, ERegister::ACCEL_XOUT_H,
                                         reinterpret_cast<uint8_t *>(&cmp),
                                         6U)) {
                this->mAcc.x = static_cast<int16_t>((cmp.x << 8U) | ((cmp.x >> 8) & 0xFF)) - mAccOffset.x;
                this->mAcc.y = static_cast<int16_t>((cmp.y << 8U) | ((cmp.y >> 8) & 0xFF)) - mAccOffset.y;
                this->mAcc.z = static_cast<int16_t>((cmp.z << 8U) | ((cmp.z >> 8) & 0xFF)) - mAccOffset.z;
            }
            this->mAcc.x = (this->mAcc.x * 2.0F) / 32768.0F;
            this->mAcc.y = (this->mAcc.y * 2.0F) / 32768.0F;
            this->mAcc.z = (this->mAcc.z * 2.0F) / 32768.0F;
            return (this->mAcc);
        }

        Vector3F Mpu9150::UpdateGyr(void) {
            Vector3 cmp;
            if (this->mI2c.ReadRegisters(this->mAddress, ERegister::GYRO_XOUT_H,
                                         reinterpret_cast<uint8_t *>(&cmp),
                                         6U)) {
                this->mGyr.x = static_cast<int16_t>((cmp.x << 8U) | ((cmp.x >> 8) & 0xFF)) - mGyrOffset.x;
                this->mGyr.y = static_cast<int16_t>((cmp.y << 8U) | ((cmp.y >> 8) & 0xFF)) - mGyrOffset.y;
                this->mGyr.z = static_cast<int16_t>((cmp.z << 8U) | ((cmp.z >> 8) & 0xFF)) - mGyrOffset.z;
            }

            this->mGyr.x = this->mGyr.x * 250.0F / 32768.0F;
            this->mGyr.y = this->mGyr.y * 250.0F / 32768.0F;
            this->mGyr.z = this->mGyr.z * 250.0F / 32768.0F;
            return (this->mGyr);
        }

        Vector3F Mpu9150::UpdateMag(void) {
            Vector3 cmp;
            if (this->mI2c.ReadRegisters(this->mAddress, ERegisterMag::XOUT_L,
                                         reinterpret_cast<uint8_t *>(&cmp),
                                         6U)) {
                this->mMag.x = static_cast<int16_t>((cmp.x << 8U) | ((cmp.x >> 8) & 0xFF)) - mMagOffset.x;
                this->mMag.y = static_cast<int16_t>((cmp.y << 8U) | ((cmp.y >> 8) & 0xFF)) - mMagOffset.y;
                this->mMag.z = static_cast<int16_t>((cmp.z << 8U) | ((cmp.z >> 8) & 0xFF)) - mMagOffset.z;

                this->mMag.x = this->mMag.x * 10.0F * 1229.0F / 4096.0F + 18.0F;
                // milliGauss (1229 microTesla per 2^12 bits, 10 mG per microTesla)
                this->mMag.y = this->mMag.y * 10.0F * 1229.0F / 4096.0F + 70.0F;
                // apply calibration offsets in mG that correspond to your environment and magnetometer
                this->mMag.z = this->mMag.z * 10.0F * 1229.0F / 4096.0F + 270.0F;
            }
            return (this->mMag);
        }

        int16_t Mpu9150::UpdateTemp(void) {
            uint8_t tmpL = 0U;
            uint8_t tmpH = 0U;

            this->mI2c.ReadRegister(this->mAddress, (uint8_t) ERegister::TEMP_OUT_H, tmpH);
            this->mI2c.ReadRegister(this->mAddress, (uint8_t) ERegister::TEMP_OUT_L, tmpL);
            this->mTmp = static_cast<int16_t>((tmpH << 8U) | ((tmpL >> 8) & 0xFF));
            this->mTmp = (this->mTmp / 340.0F) + 35U;
            return (this->mTmp);
        }
    }
}
