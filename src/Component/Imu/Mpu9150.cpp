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
              , mGyrOffset{0, -150, -65}
              , mMagOffset{0, 0, 0}
              , mAccRaw{0, 0, 0}
              , mGyrRaw{0, 0, 0}
              , mMagRaw{0, 0, 0}
              , mAcc{0, 0, 0}
              , mGyr{0, 0, 0}
              , mMag{0, 0, 0}
              , mMagRate(9U)
              , mTmp(0U)
              , mLastLoopTime(0U)
              , mAhrs()
              , mYawPitchRoll{0, 0, 0} {
        }

        Core::CoreStatus Mpu9150::Initialize(void) {
            uint8_t whoAmI = 0x00U;
            uint8_t reg = 0x00U;
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

            //reset
            this->mI2c.ReadRegister(this->mAddress, ERegister::PWR_MGMT_1, reg);
            reg |= (1U << 7U);
            this->mI2c.WriteRegister(this->mAddress, ERegister::PWR_MGMT_1, reg);

            // check device
            this->mI2c.ReadRegister(this->mAddress, ERegister::WHO_AM_I, whoAmI);

            if (whoAmI == this->mAddress - 1) {
                // setClockSource
                this->mI2c.ReadRegister(this->mAddress, ERegister::PWR_MGMT_1, reg);
                reg |= (ERegisterGyro::CLOCK_PLL_XGYRO << ERegister::PWR1_CLKSEL_BIT);
                reg &= ~(1U << (ERegister::PWR1_CLKSEL_BIT + 1U));
                reg &= ~(1U << (ERegister::PWR1_CLKSEL_BIT + 2U));
                this->mI2c.WriteRegister(this->mAddress, ERegister::PWR_MGMT_1, reg);

                // setFullScaleGyroRange
                this->mI2c.ReadRegister(this->mAddress, ERegister::GYRO_CONFIG, reg);
                reg |= (ERegisterGyro::GYRO_FS_250 << 4U);
                reg &= ~(1U << 5U);
                this->mI2c.WriteRegister(this->mAddress, ERegister::GYRO_CONFIG, reg);

                // setFullScaleAccelRange
                this->mI2c.ReadRegister(this->mAddress, ERegister::ACCEL_CONFIG, reg);
                reg |= (ERegisterAccel::ACCEL_FS_2 << 4U);
                reg &= ~(1U << 5U);
                this->mI2c.WriteRegister(this->mAddress, ERegister::ACCEL_CONFIG, reg);

                // setBandWidthLowPassFilter
                this->mI2c.ReadRegister(this->mAddress, ERegister::CONFIG, reg);
                reg &= ~(1U << 2U);
                reg |= (1U << 3U);
                reg &= ~(1U << 4U);
                this->mI2c.WriteRegister(this->mAddress, ERegister::CONFIG, reg);

                //disable sleep mode
                this->mI2c.ReadRegister(this->mAddress, ERegister::PWR_MGMT_1, reg);
                reg &= ~(1U << ERegister::PWR1_SLEEP_BIT);
                this->mI2c.WriteRegister(this->mAddress, ERegister::PWR_MGMT_1, reg);

                //set Data Ready interrupt enabled status.
                this->mI2c.ReadRegister(this->mAddress, ERegister::INT_ENABLE, reg);
                reg |= (1U << 0U);
                this->mI2c.WriteRegister(this->mAddress, ERegister::INT_ENABLE, reg);

                this->mI2c.WriteRegister(this->mAddress, ERegister::INT_PIN_CFG, 0x02);
                this->mI2c.ReadRegister(this->mAddressMag, ERegisterMag::WHO_AM_I, whoAmI);
                if (whoAmI == 0U) {
                    this->mI2c.WriteRegister(this->mAddressMag, 0x0A, 0x01);
                    success = Core::CoreStatus::CORE_OK;
                }
                success = Core::CoreStatus::CORE_OK;
            }
            return (success);
        }

        void Mpu9150::Update(const uint64_t currentTime) {
            (void) currentTime;
            if (this->IsDataReady() == true) {
                this->mMagRate++;
                this->UpdateTemp();
                this->UpdateAcc();
                this->UpdateGyr();
                if (this->mMagRate >= 10U) {
                    this->mMagRate = 0U;
                    this->UpdateMag();
                }
            }

            const uint64_t now = this->mTick.GetUs();
            const float deltaTime = ((now - this->mLastLoopTime) / 1000000.0F);
            this->mLastLoopTime = now;
            this->mAhrs.MadgwickQuaternionUpdate(this->mAcc, this->mGyr, this->mMag, deltaTime);
            this->mAhrs.GetYawPitchRoll(this->mYawPitchRoll);
        }

        Vector3 Mpu9150::UpdateAcc(void) {
            if (this->mI2c.ReadRegisters(this->mAddress, ERegister::ACCEL_XOUT_H,
                                         reinterpret_cast<uint8_t *>(&this->mAccRaw),
                                         6U)) {
                this->mAccRaw.x = ((this->mAccRaw.x << 8U) | ((this->mAccRaw.x >> 8) & 0xFF)) - mAccOffset.x;
                this->mAccRaw.y = ((this->mAccRaw.y << 8U) | ((this->mAccRaw.y >> 8) & 0xFF)) - mAccOffset.y;
                this->mAccRaw.z = ((this->mAccRaw.z << 8U) | ((this->mAccRaw.z >> 8) & 0xFF)) - mAccOffset.z;
                //+ (32768.0F / 2.0F);
            }
            this->mAcc.x = (-this->mAccRaw.x * 2.0F) / 32768.0F;
            this->mAcc.y = (-this->mAccRaw.y * 2.0F) / 32768.0F;
            this->mAcc.z = (-this->mAccRaw.z * 2.0F) / 32768.0F;
            return (this->mAccRaw);
        }

        Vector3 Mpu9150::UpdateGyr(void) {
            if (this->mI2c.ReadRegisters(this->mAddress, ERegister::GYRO_XOUT_H,
                                         reinterpret_cast<uint8_t *>(&this->mGyrRaw),
                                         6U)) {
                this->mGyrRaw.x = ((this->mGyrRaw.x << 8U) | ((this->mGyrRaw.x >> 8) & 0xFF)) - mGyrOffset.x;
                this->mGyrRaw.y = ((this->mGyrRaw.y << 8U) | ((this->mGyrRaw.y >> 8) & 0xFF)) - mGyrOffset.y;
                this->mGyrRaw.z = ((this->mGyrRaw.z << 8U) | ((this->mGyrRaw.z >> 8) & 0xFF)) - mGyrOffset.z;
            }

            this->mGyr.x = -this->mGyrRaw.x * 250.0F / 32768.0F;
            this->mGyr.y = -this->mGyrRaw.y * 250.0F / 32768.0F;
            this->mGyr.z = -this->mGyrRaw.z * 250.0F / 32768.0F;
            return (this->mGyrRaw);
        }

        Vector3 Mpu9150::UpdateMag(void) {
            uint8_t dataIsReady = 0;
            this->mI2c.ReadRegister(this->mAddressMag, 0x02, dataIsReady);
            if (dataIsReady == 1U && this->mI2c.ReadRegisters(this->mAddressMag, ERegisterMag::XOUT_L,
                                                              reinterpret_cast<uint8_t *>(&this->mMagRaw),
                                                              6U)) {
                this->mMagRaw.x = ((this->mMagRaw.x << 8U) | ((this->mMagRaw.x >> 8) & 0xFF)) - mMagOffset.x;
                this->mMagRaw.y = ((this->mMagRaw.y << 8U) | ((this->mMagRaw.y >> 8) & 0xFF)) - mMagOffset.y;
                this->mMagRaw.z = ((this->mMagRaw.z << 8U) | ((this->mMagRaw.z >> 8) & 0xFF)) - mMagOffset.z;

                this->mMag.x = -this->mMagRaw.x * 10.0F * 1229.0F / 4096.0F;
                this->mMag.y = -this->mMagRaw.y * 10.0F * 1229.0F / 4096.0F;
                this->mMag.z = -this->mMagRaw.z * 10.0F * 1229.0F / 4096.0F;

                this->mI2c.WriteRegister(this->mAddressMag, 0x0A, 0x01);
            }
            return (this->mMagRaw);
        }

        int16_t Mpu9150::UpdateTemp(void) {
            uint8_t tmpL = 0U;
            uint8_t tmpH = 0U;

            this->mI2c.ReadRegister(this->mAddress, ERegister::TEMP_OUT_H, tmpH);
            this->mI2c.ReadRegister(this->mAddress, ERegister::TEMP_OUT_L, tmpL);
            this->mTmp = static_cast<int16_t>((tmpH << 8U) | ((tmpL >> 8) & 0xFF));
            this->mTmp = (this->mTmp / 340.0F) + 35U;
            return (this->mTmp);
        }

        bool Mpu9150::IsDataReady(void) const {
            uint8_t isReady = 0U;

            this->mI2c.ReadRegister(this->mAddress, ERegister::INT_ENABLE, isReady);
            isReady &= 0x01U;
            return isReady;
        }
    }
}
