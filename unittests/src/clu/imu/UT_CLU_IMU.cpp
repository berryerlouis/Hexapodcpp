#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../../src/Cluster/Imu/ClusterImu.h"
#include "../../../../src/Misc/Maths/Geometry.h"
#include "../../../mock/cmp/MockBarometer.h"
#include "../../../mock/cmp/MockMpu9150.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster
{
    namespace Imu
    {
        class UT_CLU_IMU : public ::testing::Test {
        protected:
            UT_CLU_IMU()
                : mMockMpu9150()
                , mMockBarometer()
                , mClusterImu(mMockMpu9150, mMockBarometer) {
            }

            virtual void SetUp() {
            }

            virtual void TearDown() {
            }

            virtual ~UT_CLU_IMU() = default;

            /* Mocks */
            StrictMock<Component::Imu::MockMpu9150>         mMockMpu9150;
            StrictMock<Component::Barometer::MockBarometer> mMockBarometer;

            /* Test class */
            ClusterImu mClusterImu;
        };

        TEST_F(UT_CLU_IMU, Execute_WrongCommand_Ko) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;

            request.Build(IMU, 0x5FU);
            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), 0U);
            EXPECT_EQ(response.GetCommandId(), 0U);
            EXPECT_EQ(response.GetNbParams(), 0U);
            EXPECT_EQ(success, Core::Status::CORE_ERROR);
        }


        TEST_F(UT_CLU_IMU, Execute_ALL_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;

            request.Build(IMU, EImuCommands::ALL);
            EXPECT_CALL(mMockMpu9150, ReadAcc()).WillOnce(Return(Vector3{5, 5, 5}));
            EXPECT_CALL(mMockMpu9150, ReadGyr()).WillOnce(Return(Vector3{5, 5, 5}));
            EXPECT_CALL(mMockMpu9150, ReadMag()).WillOnce(Return(Vector3{5, 5, 5}));
            EXPECT_CALL(mMockMpu9150, ReadTemp()).WillOnce(Return(25));

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::ALL);
            EXPECT_EQ(response.GetNbParams(), 20U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_IMU, Execute_ACC_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;

            request.Build(IMU, EImuCommands::ACC);
            EXPECT_CALL(mMockMpu9150, ReadAcc()).WillOnce(Return(Vector3{5, 5, 5}));

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::ACC);
            EXPECT_EQ(response.GetNbParams(), 6U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_IMU, Execute_GYR_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;

            request.Build(IMU, EImuCommands::GYR);
            EXPECT_CALL(mMockMpu9150, ReadGyr()).WillOnce(Return(Vector3{5, 5, 5}));

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::GYR);
            EXPECT_EQ(response.GetNbParams(), 6U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_IMU, Execute_MAG_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;

            request.Build(IMU, EImuCommands::MAG);
            EXPECT_CALL(mMockMpu9150, ReadMag()).WillOnce(Return(Vector3{5, 5, 5}));

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::MAG);
            EXPECT_EQ(response.GetNbParams(), 6U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_IMU, Execute_TMP_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;

            request.Build(IMU, EImuCommands::TMP);
            EXPECT_CALL(mMockMpu9150, ReadTemp()).WillOnce(Return(10U));

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::TMP);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_IMU, Execute_YAW_PITCH_ROLL_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;
            Imu3d        imu3d;

            request.Build(IMU, EImuCommands::YAW_PITCH_ROLL);
            EXPECT_CALL(mMockMpu9150, ReadYawPitchRoll()).WillOnce(Return(imu3d));

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::YAW_PITCH_ROLL);
            EXPECT_EQ(response.GetNbParams(), 6U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_IMU, Execute_PRESSURE_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;
            int32_t      pressure = 10U;

            request.Build(IMU, EImuCommands::PRESSURE);
            EXPECT_CALL(mMockBarometer, GetPressure()).WillOnce(Return(pressure));

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::PRESSURE);
            EXPECT_EQ(response.GetNbParams(), 4U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_IMU, Execute_ALTITUDE_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;
            uint16_t     seaLevel = 10U;

            request.Build(IMU, EImuCommands::ALTITUDE);
            EXPECT_CALL(mMockBarometer, GetAltitude()).WillOnce(Return(seaLevel));

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::ALTITUDE);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_IMU, Execute_TMP_BAR_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;
            uint16_t     temp = 10U;

            request.Build(IMU, EImuCommands::TMP_BAR);
            EXPECT_CALL(mMockBarometer, GetTemp()).WillOnce(Return(temp));

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::TMP_BAR);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_IMU, Execute_CALIB_SENSOR_Start_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;

            request.Build(IMU, EImuCommands::CALIB_SENSOR);
            request.Set1ByteParam((uint8_t) SensorsImu::ACCEL);
            request.Set1ByteParam(true);
            EXPECT_CALL(mMockMpu9150, StartCalibration(SensorsImu::ACCEL)).Times(1U);

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::CALIB_SENSOR);
            EXPECT_EQ(response.GetNbParams(), 0U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_IMU, Execute_CALIB_SENSOR_Stop_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;

            request.Build(IMU, EImuCommands::CALIB_SENSOR);
            request.Set1ByteParam((uint8_t) SensorsImu::ACCEL);
            request.Set1ByteParam(false);
            EXPECT_CALL(mMockMpu9150, StopCalibration(SensorsImu::ACCEL)).Times(1U);

            success = mClusterImu.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), IMU);
            EXPECT_EQ(response.GetCommandId(), EImuCommands::CALIB_SENSOR);
            EXPECT_EQ(response.GetNbParams(), 0U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }
    } // namespace Imu
} // namespace Cluster
