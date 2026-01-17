#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/drv/MockTwi.h"

#include "../../../../src/Component/Imu/Mpu9150.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::SetArrayArgument;
using ::testing::StrictMock;

namespace Component
{
    namespace Imu
    {
        class UT_CMP_MPU9150 : public ::testing::Test {
        protected:
            UT_CMP_MPU9150()
                : mMockTwi()
                , mMpu9150(mMockTwi) {
            }

            virtual void SetUp() {
                Core::Status success = Core::Status::CORE_ERROR;

                EXPECT_CALL(mMockTwi, ReadRegister(_, _, _))
                        .WillRepeatedly(Return(true));
                EXPECT_CALL(mMockTwi, WriteRegister(_, _, _))
                        .WillRepeatedly(Return(true));

                EXPECT_CALL(mMockTwi,
                            ReadRegister(Mpu9150::MPU9150_I2C_ADDRESS,
                                         Mpu9150::ERegister::WHO_AM_I,
                                         _))
                        .WillOnce(
                                DoAll(SetArgReferee<2U>(
                                              Mpu9150::MPU9150_I2C_ADDRESS - 1),
                                      Return(true)));
                EXPECT_CALL(mMockTwi,
                            ReadRegister(Mpu9150::AK8963_I2C_ADDRESS, 0, _))
                        .WillOnce(
                                DoAll(SetArgReferee<2U>(0x48U), Return(true)));
                EXPECT_CALL(
                        mMockTwi,
                        ReadRegisters(Mpu9150::AK8963_I2C_ADDRESS, 0x10, _, 3))
                        .WillRepeatedly(Return(true));
                success = mMpu9150.Initialize();

                EXPECT_EQ(success, Core::Status::CORE_OK);
            }

            virtual void TearDown() {
            }

            virtual ~UT_CMP_MPU9150() = default;

            /* Mocks */
            StrictMock<Driver::Twi::MockTwi> mMockTwi;

            /* Test class */
            Mpu9150 mMpu9150;
        };

        TEST_F(UT_CMP_MPU9150, Update_Ok) {
            mMpu9150.Update(0UL);
        }

        TEST_F(UT_CMP_MPU9150, UpdateNoCalib_Ok) {
            EXPECT_CALL(mMockTwi,
                        ReadRegister(_, Mpu9150::ERegister::INT_ENABLE, _))
                    .WillOnce(DoAll(SetArgReferee<2U>(true), Return(true)));
            EXPECT_CALL(
                    mMockTwi,
                    ReadRegisters(_, Mpu9150::ERegister::ACCEL_XOUT_H, _, 14U))
                    .WillOnce(Return(true));

            EXPECT_CALL(mMockTwi, ReadRegister(_, 0x02U, _))
                    .WillOnce(DoAll(SetArgReferee<2U>(1U), Return(true)));
            EXPECT_CALL(mMockTwi, ReadRegisters(_, 0x03U, _, 6U))
                    .WillOnce(Return(true));
            EXPECT_CALL(mMockTwi, WriteRegister(_, 0x0AU, 0x01U))
                    .WillOnce(Return(true));

            EXPECT_CALL(mMockTwi,
                        ReadRegister(_, Mpu9150::ERegister::TEMP_OUT_H, _))
                    .WillOnce(Return(true));
            EXPECT_CALL(mMockTwi,
                        ReadRegister(_, Mpu9150::ERegister::TEMP_OUT_L, _))
                    .WillOnce(Return(true));
            mMpu9150.Update(0UL);
        }

        TEST_F(UT_CMP_MPU9150, UpdateCalibAcc_Ok) {
            mMpu9150.StartCalibration(SensorsImu::ACCEL);
            EXPECT_CALL(
                    mMockTwi,
                    ReadRegisters(_, Mpu9150::ERegister::ACCEL_XOUT_H, _, 6U))
                    .WillOnce(Return(true));

            mMpu9150.Update(0UL);
        }

        TEST_F(UT_CMP_MPU9150, UpdateCalibAcc100_Ok) {
            mMpu9150.StartCalibration(SensorsImu::ACCEL);
            for (size_t i = 0U; i < 99U; i++) {
                EXPECT_CALL(mMockTwi,
                            ReadRegisters(
                                    _, Mpu9150::ERegister::ACCEL_XOUT_H, _, 6U))
                        .WillOnce(Return(true));

                mMpu9150.Update(0UL);
            }

            EXPECT_CALL(
                    mMockTwi,
                    ReadRegisters(_, Mpu9150::ERegister::ACCEL_XOUT_H, _, 6U))
                    .WillOnce(Return(true));
            mMpu9150.Update(0UL);
        }

        TEST_F(UT_CMP_MPU9150, UpdateCalibGyr_Ok) {
            mMpu9150.StartCalibration(SensorsImu::GYRO);
            EXPECT_CALL(
                    mMockTwi,
                    ReadRegisters(_, Mpu9150::ERegister::GYRO_XOUT_H, _, 6U))
                    .WillOnce(Return(true));

            mMpu9150.Update(0UL);
        }

        TEST_F(UT_CMP_MPU9150, UpdateCalibGyr100_Ok) {
            mMpu9150.StartCalibration(SensorsImu::GYRO);
            for (size_t i = 0U; i < 99U; i++) {
                EXPECT_CALL(mMockTwi,
                            ReadRegisters(
                                    _, Mpu9150::ERegister::GYRO_XOUT_H, _, 6U))
                        .WillOnce(Return(true));

                mMpu9150.Update(0UL);
            }

            EXPECT_CALL(
                    mMockTwi,
                    ReadRegisters(_, Mpu9150::ERegister::GYRO_XOUT_H, _, 6U))
                    .WillOnce(Return(true));
            mMpu9150.Update(0UL);
        }

        TEST_F(UT_CMP_MPU9150, UpdateCalibMag_Ok) {
            uint8_t magRaw[6U] = {0U};
            mMpu9150.StartCalibration(SensorsImu::MAG);
            EXPECT_CALL(mMockTwi, ReadRegister(_, 0x02U, _))
                    .WillOnce(DoAll(SetArgReferee<2U>(1U), Return(true)));
            EXPECT_CALL(mMockTwi, ReadRegisters(_, 0x03U, _, 6U))
                    .WillOnce(DoAll(SetArrayArgument<2U>(magRaw, magRaw + 6U),
                                    Return(true)));
            EXPECT_CALL(mMockTwi, WriteRegister(_, 0x0AU, 0x01U))
                    .WillOnce(Return(true));

            mMpu9150.Update(0UL);
        }


        TEST_F(UT_CMP_MPU9150, UpdateCalibMag100_Ok) {
            uint8_t magRaw[6U] = {0U};
            mMpu9150.StartCalibration(SensorsImu::MAG);
            for (size_t i = 0U; i < 999U; i++) {
                EXPECT_CALL(mMockTwi, ReadRegister(_, 0x02U, _))
                        .WillOnce(DoAll(SetArgReferee<2U>(1U), Return(true)));
                EXPECT_CALL(mMockTwi, ReadRegisters(_, 0x03U, _, 6U))
                        .WillOnce(
                                DoAll(SetArrayArgument<2U>(magRaw, magRaw + 6U),
                                      Return(true)));
                EXPECT_CALL(mMockTwi, WriteRegister(_, 0x0AU, 0x01U))
                        .WillOnce(Return(true));

                mMpu9150.Update(0UL);
            }

            EXPECT_CALL(mMockTwi, ReadRegister(_, 0x02U, _))
                    .WillOnce(DoAll(SetArgReferee<2U>(1U), Return(true)));
            EXPECT_CALL(mMockTwi, ReadRegisters(_, 0x03U, _, 6U))
                    .WillOnce(DoAll(SetArrayArgument<2U>(magRaw, magRaw + 6U),
                                    Return(true)));
            EXPECT_CALL(mMockTwi, WriteRegister(_, 0x0AU, 0x01U))
                    .WillOnce(Return(true));
            mMpu9150.Update(0UL);
        }

        TEST_F(UT_CMP_MPU9150, ReadAcc_Ok) {
            mMpu9150.ReadAcc();
        }

        TEST_F(UT_CMP_MPU9150, ReadGyr_Ok) {
            mMpu9150.ReadGyr();
        }

        TEST_F(UT_CMP_MPU9150, ReadMag_Ok) {
            mMpu9150.ReadMag();
        }

        TEST_F(UT_CMP_MPU9150, ReadTemp_Ok) {
            mMpu9150.ReadTemp();
        }

        TEST_F(UT_CMP_MPU9150, ReadYawPitchRoll_Ok) {
            mMpu9150.ReadYawPitchRoll();
        }
    } // namespace Imu
} // namespace Component
