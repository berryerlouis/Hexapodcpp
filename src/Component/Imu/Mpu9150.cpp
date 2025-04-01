#include "unistd.h"
#include "Mpu9150.h"
#ifdef RPI
#include "wiringPi/wiringPiI2C.h"
#endif

namespace Component
{
    namespace Imu
    {
#define NB_SAMPLES_ACC_GYR  100U
#define NB_SAMPLES_MAG      1000U

        Vector3F accRawCalib;
        Vector3F gyrRawCalib;


        Vector3 accSign = {-1, 1, 1};
        Vector3 gyrSign = {1, -1, -1};
        Vector3 magSign = {-1, -1, 1};

        Mpu9150::Mpu9150(Twi::TwiInterface &i2c, Tick::TickInterface &tick, const uint8_t address) :
            mI2c(i2c)
            , mTick(tick)
            , mAddress(address)
            , mAddressMag(AK8963_I2C_ADDRESS)
            , mAccOffset{0, 0, 0}
            , mGyrOffset{0, 0, 0}
            , mMagOffset{47.5F, 71.0F, -54.5}
            , mMagBias{0, 0, 0}
            , mIndexCalib(0U)
            , mStartCalib(false)
            , mSensorToCalib(NONE)
            , mMagCalibMin{100000, 100000, 100000}
            , mMagCalibMax{-100000, -100000, -100000}
            , mAcc{0, 0, 0}
            , mGyr{0, 0, 0}
            , mMag{0, 0, 0}
            , mTmp(0U)
            , mLastLoopTime(0U)
            , mAhrs()
            , mYawPitchRoll{0, 0, 0}
            , mDoComputation(false) {
#ifdef RPI
            this->mAddress = wiringPiI2CSetup(address);
            this->mAddressMag = wiringPiI2CSetup(AK8963_I2C_ADDRESS);
#endif
        }

        Core::Status Mpu9150::Initialize(void) {
            uint8_t whoAmI = 0x00U;
            Core::Status success = Core::Status::CORE_ERROR;

            // check device
            this->mI2c.ReadRegister(this->mAddress, ERegister::WHO_AM_I, whoAmI);

            if (whoAmI == MPU9150_I2C_ADDRESS - 1U) {
                uint8_t reg = 0x00U;
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

                // disable sleep mode
                this->mI2c.ReadRegister(this->mAddress, ERegister::PWR_MGMT_1, reg);
                reg &= ~(1U << ERegister::PWR1_SLEEP_BIT);
                this->mI2c.WriteRegister(this->mAddress, ERegister::PWR_MGMT_1, reg);

                // set Data Ready interrupt enabled status.
                this->mI2c.ReadRegister(this->mAddress, ERegister::INT_ENABLE, reg);
                reg |= (1U << 0U);
                this->mI2c.WriteRegister(this->mAddress, ERegister::INT_ENABLE, reg);

                //Connect Magnetometer
                this->mI2c.WriteRegister(this->mAddress, ERegister::INT_PIN_CFG, 0x02);
                usleep(2000);
                this->mI2c.ReadRegister(this->mAddressMag, ERegisterMag::WHO_AM_I, whoAmI);

                if (whoAmI == 0x48U) {
                    this->mI2c.WriteRegister(this->mAddressMag, 0x0A, 0x0F);
                    this->AdjustingMag();
                    this->mI2c.WriteRegister(this->mAddressMag, 0x0A, 0x01);
                    success = Core::Status::CORE_OK;
                }
            }
            return (success);
        }

        void Mpu9150::Update(const uint64_t currentTime) {
            (void) currentTime;
            static float deltaTime;
            if (this->mStartCalib == false) {
                if (this->IsDataReady() == true && this->mDoComputation == false) {
                    this->UpdateAcc();
                    this->UpdateGyr();
                    this->UpdateMag();
                    this->UpdateTemp();

                    const uint64_t now = this->mTick.GetUs();
                    deltaTime = ((now - this->mLastLoopTime) / 1000000.0F);
                    this->mLastLoopTime = now;
                    this->mDoComputation = true;
                } else if (this->mDoComputation == true) {
                    // convert to radian gyrometer measurement
                    Vector3F gyr = this->mGyr;
                    gyr.x *= M_PI / 180.0F;
                    gyr.y *= M_PI / 180.0F;
                    gyr.z *= M_PI / 180.0F;
                    this->mAhrs.Update(this->mAcc, gyr, this->mMag, deltaTime);
                    this->mAhrs.GetRollPitchYaw(this->mYawPitchRoll);
                    this->mDoComputation = false;
                }
            } else {
                if (this->mSensorToCalib == ACCEL) {
                    this->UpdateAcc();
                    accRawCalib.x += this->mAcc.x;
                    accRawCalib.y += this->mAcc.y;
                    accRawCalib.z += this->mAcc.z;
                } else if (this->mSensorToCalib == GYRO) {
                    this->UpdateGyr();
                    gyrRawCalib.x += this->mGyr.x;
                    gyrRawCalib.y += this->mGyr.y;
                    gyrRawCalib.z += this->mGyr.z;
                } else if (this->mSensorToCalib == MAG) {
                    this->UpdateMag();
                    if (this->mMag.x < this->mMagCalibMin.x) {
                        this->mMagCalibMin.x = this->mMag.x;
                    } else if (this->mMag.x > this->mMagCalibMax.x) {
                        this->mMagCalibMax.x = this->mMag.x;
                    }
                    if (this->mMag.y < this->mMagCalibMin.y) {
                        this->mMagCalibMin.y = this->mMag.y;
                    } else if (this->mMag.y > this->mMagCalibMax.y) {
                        this->mMagCalibMax.y = this->mMag.y;
                    }
                    if (this->mMag.z < this->mMagCalibMin.z) {
                        this->mMagCalibMin.z = this->mMag.z;
                    } else if (this->mMag.z > this->mMagCalibMax.z) {
                        this->mMagCalibMax.z = this->mMag.z;
                    }
                }
                if (++this->mIndexCalib == (this->mSensorToCalib == MAG ? NB_SAMPLES_MAG : NB_SAMPLES_ACC_GYR)) {
                    this->mStartCalib = false;
                    this->StartCalibration(this->mSensorToCalib, false);
                }
            }
        }

        void Mpu9150::StartCalibration(const SensorsImu sensor, const bool enable) {
            this->mStartCalib = enable;
            this->mSensorToCalib = sensor;
            if (this->mStartCalib == true) {
                this->mIndexCalib = 0U;
                if (this->mSensorToCalib == ACCEL) {
                    this->mAccOffset = {0, 0, 0};
                } else if (this->mSensorToCalib == GYRO) {
                    this->mGyrOffset = {0, 0, 0};
                } else if (this->mSensorToCalib == MAG) {
                    this->mMagCalibMin = {100000, 100000, 100000};
                    this->mMagCalibMax = {0, 0, 0};
                }
            } else {
                if (this->mSensorToCalib == ACCEL) {
                    mAccOffset.x = accRawCalib.x / NB_SAMPLES_ACC_GYR;
                    mAccOffset.y = accRawCalib.y / NB_SAMPLES_ACC_GYR;
                    mAccOffset.z = accRawCalib.z / NB_SAMPLES_ACC_GYR;
                } else if (this->mSensorToCalib == GYRO) {
                    mGyrOffset.x = gyrRawCalib.x / NB_SAMPLES_ACC_GYR;
                    mGyrOffset.y = gyrRawCalib.y / NB_SAMPLES_ACC_GYR;
                    mGyrOffset.z = gyrRawCalib.z / NB_SAMPLES_ACC_GYR;
                } else if (this->mSensorToCalib == MAG) {
                    this->mMagOffset.x = (this->mMagCalibMax.x + this->mMagCalibMin.x) / 2U;
                    this->mMagOffset.y = (this->mMagCalibMax.y + this->mMagCalibMin.y) / 2U;
                    this->mMagOffset.z = (this->mMagCalibMax.z + this->mMagCalibMin.z) / 2U;
                }
                this->mSensorToCalib = NONE;
            }
        }

        void Mpu9150::UpdateAll(void) {
            uint8_t data[14U] = {0};
            if (this->mI2c.ReadRegisters(this->mAddress, ERegister::ACCEL_XOUT_H,
                                         reinterpret_cast<uint8_t *>(&data), 14U)) {
                Vector3 accRaw;
                accRaw.x = ((data[0U] << 8U) | ((data[1U]))) - mAccOffset.x;
                accRaw.y = ((data[2U] << 8U) | ((data[3U]))) - mAccOffset.y;
                accRaw.z = ((data[4U] << 8U) | ((data[5U]))) - mAccOffset.z;
                this->mAcc.x = (accRaw.x * 2.0F) / 32768.0F;
                this->mAcc.y = (accRaw.y * 2.0F) / 32768.0F;
                this->mAcc.z = (accRaw.z * 2.0F) / 32768.0F;

                Vector3 gyrRaw;
                gyrRaw.x = ((data[8U] << 8U) | ((data[9U]))) - mGyrOffset.x;
                gyrRaw.y = ((data[10U] << 8U) | ((data[11U]))) - mGyrOffset.y;
                gyrRaw.z = ((data[12U] << 8U) | ((data[13U]))) - mGyrOffset.z;
                this->mGyr.x = (gyrRaw.x * 250.0F) / 32768.0F;
                this->mGyr.y = (gyrRaw.y * 250.0F) / 32768.0F;
                this->mGyr.z = (gyrRaw.z * 250.0F) / 32768.0F;

                this->mTmp = static_cast<uint8_t>((((data[6U] << 8U) | ((data[7U]))) / 340.0F) + 36.5F);
            }
        }

        Vector3 Mpu9150::UpdateAcc(void) {
            Vector3 accRaw;
            if (this->mI2c.ReadRegisters(this->mAddress, ERegister::ACCEL_XOUT_H,
                                         reinterpret_cast<uint8_t *>(&accRaw), 6U)) {
                accRaw.x = ((accRaw.x & 0x00FF) << 8U | ((accRaw.x & 0xFF00U) >> 8U)) - mAccOffset.x;
                accRaw.y = ((accRaw.y & 0x00FF) << 8U | ((accRaw.y & 0xFF00U) >> 8U)) - mAccOffset.y;
                accRaw.z = ((accRaw.z & 0x00FF) << 8U | ((accRaw.z & 0xFF00U) >> 8U)) - mAccOffset.z;
            }
            constexpr float res = 2.0F / 32768.0F;
            this->mAcc.x = accRaw.x * res * accSign.x;
            this->mAcc.y = accRaw.y * res * accSign.y;
            this->mAcc.z = accRaw.z * res * accSign.z;
            return (accRaw);
        }

        Vector3 Mpu9150::UpdateGyr(void) {
            Vector3 gyrRaw;
            if (this->mI2c.ReadRegisters(this->mAddress, ERegister::GYRO_XOUT_H,
                                         reinterpret_cast<uint8_t *>(&gyrRaw), 6U)) {
                gyrRaw.x = ((gyrRaw.x & 0x00FF) << 8U | ((gyrRaw.x & 0xFF00U) >> 8U)) - mGyrOffset.x;
                gyrRaw.y = ((gyrRaw.y & 0x00FF) << 8U | ((gyrRaw.y & 0xFF00U) >> 8U)) - mGyrOffset.y;
                gyrRaw.z = ((gyrRaw.z & 0x00FF) << 8U | ((gyrRaw.z & 0xFF00U) >> 8U)) - mGyrOffset.z;
            }
            constexpr float res = 250.0F / 32768.0F;
            this->mGyr.x = gyrRaw.x * res * gyrSign.x;
            this->mGyr.y = gyrRaw.y * res * gyrSign.y;
            this->mGyr.z = gyrRaw.z * res * gyrSign.z;
            return (gyrRaw);
        }

        void Mpu9150::AdjustingMag(void) {
            int8_t adjustMagValues[3U] = {0, 0, 0};
            this->mI2c.ReadRegisters(this->mAddressMag, ERegisterMag::ASAX,
                                     reinterpret_cast<uint8_t *>(&adjustMagValues), 3U);
            this->mMagBias.x = ((adjustMagValues[0U] - 128.0F) / 256.0F) + 1.0F;
            this->mMagBias.y = ((adjustMagValues[1U] - 128.0F) / 256.0F) + 1.0F;
            this->mMagBias.z = ((adjustMagValues[2U] - 128.0F) / 256.0F) + 1.0F;
        }

        Vector3 Mpu9150::UpdateMag(void) {
            uint8_t dataIsReady = 0;
            Vector3 magRaw;

            this->mI2c.ReadRegister(this->mAddressMag, 0x02, dataIsReady);
            if (dataIsReady == 1U && this->mI2c.ReadRegisters(this->mAddressMag, ERegisterMag::XOUT_L,
                                                              reinterpret_cast<uint8_t *>(&magRaw), 6U)) {
                if ((abs(magRaw.x) + abs(magRaw.y) + abs(magRaw.z)) >= 4912.0) {
                    return (magRaw);
                }
                constexpr float res = 10.0F * 1229.0F / 4096.0F;
                if (this->mStartCalib == false) {
                    this->mMag.x = ((magRaw.x - this->mMagOffset.x) * res * this->mMagBias.x) * magSign.x;
                    this->mMag.y = ((magRaw.y - this->mMagOffset.y) * res * this->mMagBias.y) * magSign.y;
                    this->mMag.z = ((magRaw.z - this->mMagOffset.z) * res * this->mMagBias.z) * magSign.z;
                } else {
                    this->mMag.x = magRaw.x;
                    this->mMag.y = magRaw.y;
                    this->mMag.z = magRaw.z;
                }

                this->mI2c.WriteRegister(this->mAddressMag, 0x0A, 0x01);
            }
            return (magRaw);
        }

        int16_t Mpu9150::UpdateTemp(void) {
            uint8_t tmpL = 0U;
            uint8_t tmpH = 0U;

            this->mI2c.ReadRegister(this->mAddress, ERegister::TEMP_OUT_H, tmpH);
            this->mI2c.ReadRegister(this->mAddress, ERegister::TEMP_OUT_L, tmpL);
            this->mTmp = static_cast<int16_t>((tmpH << 8U) | tmpL);
            this->mTmp = (this->mTmp / 340.0F) + 36.5F;
            return (this->mTmp);
        }

        bool Mpu9150::IsDataReady(void) const {
            uint8_t isReady = 0U;

            this->mI2c.ReadRegister(this->mAddress, ERegister::INT_ENABLE, isReady);
            isReady &= 0x01U;
            return isReady;
        }

        Vector3 Mpu9150::ReadAcc(void) const {
            return (Vector3{.x = static_cast<int16_t>(this->mAcc.x * 100),
                            .y = static_cast<int16_t>(this->mAcc.y * 100),
                            .z = static_cast<int16_t>(this->mAcc.z * 100)});
        }

        Vector3 Mpu9150::ReadGyr(void) const {
            return (Vector3{.x = static_cast<int16_t>(this->mGyr.x * 10),
                            .y = static_cast<int16_t>(this->mGyr.y * 10),
                            .z = static_cast<int16_t>(this->mGyr.z * 10)});
        }

        Vector3 Mpu9150::ReadMag(void) const {
            return (Vector3{.x = static_cast<int16_t>(this->mMag.x),
                            .y = static_cast<int16_t>(this->mMag.y),
                            .z = static_cast<int16_t>(this->mMag.z)});
        }

        int16_t Mpu9150::ReadTemp(void) const {
            return (this->mTmp * 10);
        }
    } // namespace Imu
} // namespace Component
