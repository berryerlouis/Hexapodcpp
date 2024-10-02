#pragma once


#include "../ClusterBase.h"
#include "../../Component/Imu/Mpu9150Interface.h"
#include "../../Misc/Maths/Geometry.h"

namespace Cluster
{
    namespace Imu
    {
        using namespace Component::Imu;

        class ClusterImu : public ClusterBase, StrategyCluster {
        public:
            ClusterImu(Mpu9150Interface &imu)
                : ClusterBase(IMU, this)
                  , StrategyCluster(NB_COMMANDS_IMU)
                  , mImu(imu) {
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::ALL, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::ACC, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::GYR, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::MAG, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::TMP, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::YAW_PITCH_ROLL, .expectedSize = 0U});
            }

            ~ClusterImu() = default;

            virtual Core::CoreStatus ExecuteFrame(const Frame &request, Frame &response) override {
                Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
                if (request.commandId == EImuCommands::ALL) {
                    const Vector3F acc = this->mImu.ReadAcc();
                    const Vector3F gyr = this->mImu.ReadGyr();
                    const Vector3F mag = this->mImu.ReadMag();
                    const uint16_t temp = this->mImu.ReadTemp();
                    success = this->BuildFrameAll(acc, gyr, mag, temp, response);
                } else if (request.commandId == EImuCommands::ACC) {
                    const Vector3F acc = this->mImu.ReadAcc();
                    success = this->BuildFrameAcc(acc, response);
                } else if (request.commandId == EImuCommands::GYR) {
                    const Vector3F gyr = this->mImu.ReadGyr();
                    success = this->BuildFrameGyr(gyr, response);
                } else if (request.commandId == EImuCommands::MAG) {
                    const Vector3F mag = this->mImu.ReadMag();
                    success = this->BuildFrameMag(mag, response);
                } else if (request.commandId == EImuCommands::TMP) {
                    const uint16_t temp = this->mImu.ReadTemp();
                    success = this->BuildFrameTmp(temp, response);
                } else if (request.commandId == EImuCommands::YAW_PITCH_ROLL) {
                    const Position3D ypr = this->mImu.ReadYawPitchRoll();
                    success = this->BuildFrameYawPitchRoll(ypr, response);
                }
                return success;
            }

            inline Core::CoreStatus BuildFrameAll(const Vector3F &acc, const Vector3F &gyr, const Vector3F &mag,
                                                  const uint16_t temp, Frame &response) const {
                Vector3 cmp1;
                Vector3 cmp2;
                Vector3 cmp3;
                cmp1.x = static_cast<int16_t>(acc.x);
                cmp1.y = static_cast<int16_t>(acc.y);
                cmp1.z = static_cast<int16_t>(acc.z);
                cmp2.x = static_cast<int16_t>(gyr.x);
                cmp2.y = static_cast<int16_t>(gyr.y);
                cmp2.z = static_cast<int16_t>(gyr.z);
                cmp3.x = static_cast<int16_t>(mag.x);
                cmp3.y = static_cast<int16_t>(mag.y);
                cmp3.z = static_cast<int16_t>(mag.z);
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ALL);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.SetxBytesParam(6U, (uint8_t *) &cmp1);
                    response.SetxBytesParam(6U, (uint8_t *) &cmp2);
                    response.SetxBytesParam(6U, (uint8_t *) &cmp3);
                    response.Set2BytesParam(temp);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameAcc(const Vector3F &acc, Frame &response) const {
                Vector3 cmp;
                cmp.x = static_cast<int16_t>(acc.x);
                cmp.y = static_cast<int16_t>(acc.y);
                cmp.z = static_cast<int16_t>(acc.z);
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ACC);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.SetxBytesParam(6U, (uint8_t *) &cmp);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGyr(const Vector3F &gyr, Frame &response) const {
                Vector3 cmp;
                cmp.x = static_cast<int16_t>(gyr.x);
                cmp.y = static_cast<int16_t>(gyr.y);
                cmp.z = static_cast<int16_t>(gyr.z);
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::GYR);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.SetxBytesParam(6U, (uint8_t *) &cmp);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameMag(const Vector3F &mag, Frame &response) const {
                Vector3 cmp;
                cmp.x = static_cast<int16_t>(mag.x);
                cmp.y = static_cast<int16_t>(mag.y);
                cmp.z = static_cast<int16_t>(mag.z);
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::MAG);
                if (success == Core::CoreStatus::CORE_OK) {
                    response.SetxBytesParam(6U, (uint8_t *) &cmp);
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

        private:
            Mpu9150Interface &mImu;
        };
    }
}
