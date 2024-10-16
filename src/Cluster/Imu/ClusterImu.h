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
                  , mImu(imu) {
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::ALL, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::ACC, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::GYR, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::MAG, .expectedSize = 0U});
                this->AddClusterItem((ClusterItem){.commandId = EImuCommands::TMP, .expectedSize = 0U});
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
                }
                return success;
            }

            inline Core::CoreStatus BuildFrameAll(const Vector3 &acc, const Vector3 &gyr, const Vector3 &mag,
                                                  const uint16_t temp, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::ALL);
                if (success) {
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
                if (success) {
                    response.SetxBytesParam(6U, (uint8_t *) &acc);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameGyr(const Vector3 &gyr, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::GYR);
                if (success) {
                    response.SetxBytesParam(6U, (uint8_t *) &gyr);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameMag(const Vector3 &mag, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::MAG);
                if (success) {
                    response.SetxBytesParam(6U, (uint8_t *) &mag);
                }
                return (success);
            }

            inline Core::CoreStatus BuildFrameTmp(const uint16_t temp, Frame &response) const {
                const Core::CoreStatus success = response.Build(
                    EClusters::IMU,
                    EImuCommands::TMP);
                if (success) {
                    response.Set2BytesParam(temp);
                }
                return (success);
            }

        private:
            Mpu9150Interface &mImu;
        };
    }
}
