#pragma once


#include "../ClusterBase.h"
#include "../../Component/Barometer/BarometerInterface.h"
#include "../../Component/Imu/Mpu9150Interface.h"
#include "../../Misc/Maths/Geometry.h"

namespace Cluster
{
    namespace Imu
    {
        using namespace Component::Imu;
        using namespace Component::Barometer;

        class ClusterImu : public ClusterBase, StrategyCluster {
        public:
            ClusterImu(Mpu9150Interface &imu, BarometerInterface &barometer)
                : ClusterBase(IMU, this)
                  , StrategyCluster(NB_COMMANDS_IMU)
                  , mImu(imu)
                  , mBarometer(barometer) {
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::ALL, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::ACC, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::GYR, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::MAG, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::TMP, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::YAW_PITCH_ROLL, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::PRESSURE, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::ALTITUDE, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::TMP_BAR, .expectedSize = 0U});
            }

            ~ClusterImu() = default;

            virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
                if (request.commandId == EImuCommands::ALL) {
                    const Vector3 acc = this->mImu.ReadAcc();
                    const Vector3 gyr = this->mImu.ReadGyr();
                    const Vector3 mag = this->mImu.ReadMag();
                    const uint16_t temp = this->mImu.ReadTemp();
                    success = this->BuildFrameAll(acc, gyr, mag, temp, response);
                } else if (request.commandId == EImuCommands::ACC) {
                    const Vector3 acc = this->mImu.ReadAcc();
                    success = this->BuildFrameAcc(acc, response);
                } else if (request.commandId == EImuCommands::GYR) {
                    const Vector3 gyr = this->mImu.ReadGyr();
                    success = this->BuildFrameGyr(gyr, response);
                } else if (request.commandId == EImuCommands::MAG) {
                    const Vector3 mag = this->mImu.ReadMag();
                    success = this->BuildFrameMag(mag, response);
                } else if (request.commandId == EImuCommands::TMP) {
                    const uint16_t temp = this->mImu.ReadTemp();
                    success = this->BuildFrameTmp(temp, response);
                } else if (request.commandId == EImuCommands::YAW_PITCH_ROLL) {
                    const Position3D ypr = this->mImu.ReadYawPitchRoll();
                    success = this->BuildFrameYawPitchRoll(ypr, response);
                } else if (request.commandId == EImuCommands::PRESSURE) {
                    const int32_t pressure = this->mBarometer.GetPressure();
                    success = this->BuildFramePressure(pressure, response);
                } else if (request.commandId == EImuCommands::ALTITUDE) {
                    const uint16_t seaLevel = this->mBarometer.GetAltitude();
                    success = this->BuildFrameSeaLevel(seaLevel, response);
                } else if (request.commandId == EImuCommands::TMP_BAR) {
                    const int16_t temp = this->mBarometer.GetTemp();
                    success = this->BuildFrameTmpBar(temp, response);
                }
                return success;
            }

            inline Core::CoreStatus BuildFrameAll(const Vector3 &acc, const Vector3 &gyr, const Vector3 &mag,
                                                  const uint16_t temp, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ALL);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.SetxBytesParam(6U, (uint8_t *) &acc);
                    response.SetxBytesParam(6U, (uint8_t *) &gyr);
                    response.SetxBytesParam(6U, (uint8_t *) &mag);
                    response.Set2BytesParam(temp);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameAcc(const Vector3 &acc, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ACC);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.SetxBytesParam(6U, (uint8_t *) &acc);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGyr(const Vector3 &gyr, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::GYR);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.SetxBytesParam(6U, (uint8_t *) &gyr);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameMag(const Vector3 &mag, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::MAG);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.SetxBytesParam(6U, (uint8_t *) &mag);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameTmp(const uint16_t temp, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::TMP);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set2BytesParam(temp);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameYawPitchRoll(const Position3D ypr, Frame &response) const {
                Vector3 cmp;
                cmp.x = static_cast<int16_t>(ypr.roll);
                cmp.y = static_cast<int16_t>(ypr.pitch);
                cmp.z = static_cast<int16_t>(ypr.yaw);
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::YAW_PITCH_ROLL);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.SetxBytesParam(6U, (uint8_t *) &cmp);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFramePressure(const int32_t pressure, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::PRESSURE);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set4BytesParam(pressure);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameSeaLevel(const uint16_t seaLevel, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ALTITUDE);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set2BytesParam(seaLevel);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameTmpBar(const int16_t temp, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::TMP_BAR);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.Set2BytesParam(temp);
                }
                return (success);
            }

        private:
            Mpu9150Interface &mImu;
            BarometerInterface &mBarometer;
        };
    }
}
